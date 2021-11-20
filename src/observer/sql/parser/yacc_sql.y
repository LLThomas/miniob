
%{
//bison -d -b yacc_sql yacc_sql.y
#include "sql/parser/parse_defs.h"
#include "sql/parser/yacc_sql.tab.h"
#include "sql/parser/lex.yy.h"
// #include "common/log/log.h" // 包含C++中的头文件

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct ParserContext {
  Query *ssql;

  size_t query_length;
  Query *queries[MAX_NUM];
  size_t last_condition_length_length;
  // 每次 begin_select 会记录当前的 condition_length 到 last_condition_length
  size_t last_condition_length[MAX_NUM];

  size_t condition_length;
  Condition conditions[MAX_NUM];
  size_t value_length;
  Value values[MAX_NUM];
  size_t tuple_length;
  InsertTuple tuples[MAX_NUM];

  CompOp comp;
  char id[MAX_NUM];
  FuncName func;
} ParserContext;

//获取子串
char *substr(const char *s,int n1,int n2)/*从s中提取下标为n1~n2的字符组成一个新字符串，然后返回这个新串的首地址*/
{
  char *sp = malloc(sizeof(char) * (n2 - n1 + 2));
  int i, j = 0;
  for (i = n1; i <= n2; i++) {
    sp[j++] = s[i];
  }
  sp[j] = 0;
  return sp;
}

void yyerror(yyscan_t scanner, const char *str)
{
  ParserContext *context = (ParserContext *)(yyget_extra(scanner));
  query_reset(context->ssql);
  context->ssql->flag = SCF_ERROR;
  context->condition_length = 0;
  context->value_length = 0;
  context->tuple_length = 0;
  context->query_length = 0;
  context->last_condition_length_length = 0;
  printf("parse sql failed. error=%s", str);
}

ParserContext *get_context(yyscan_t scanner)
{
  return (ParserContext *)yyget_extra(scanner);
}

#define CONTEXT get_context(scanner)

%}

%define api.pure full
%lex-param { yyscan_t scanner }
%parse-param { void *scanner }

//标识tokens
%token  SEMICOLON
        CREATE
        DROP
        TABLE
        TABLES
        INDEX
        SELECT
        DESC
        SHOW
        SYNC
        INSERT
        DELETE
        UPDATE
        LBRACE
        RBRACE
        COMMA
        TRX_BEGIN
        TRX_COMMIT
        TRX_ROLLBACK
        INT_T
        DATE_T
        STRING_T
        FLOAT_T
        TEXT_T
        HELP
        EXIT
        DOT //QUOTE
        INTO
        VALUES
        FROM
        WHERE
        AND
        SET
        ON
        LOAD
        DATA
        INFILE
        EQ
        LT
        GT
        LE
        GE
        NE
		AGGMAX
		AGGMIN
		AGGCOUNT
		AGGAVG
		NOT
		NULL_TOK
		NULLABLE
		IS
		UNIQUE
		INNER
		JOIN
		GROUP
		BY
		ASC
		ORDER
		IN

%code requires { #include <stdbool.h> }

%union {
  struct _Attr *attr;
  struct _Condition *condition1;
  struct _Value *value1;
  char *string;
  int number;
  float floats;
  char *position;
  bool *bools;
}

%token <number> NUMBER
%token <floats> FLOAT
%token <string> ID
%token <string> PATH
%token <string> SSS
%token <string> STAR
%token <string> STRING_V
//非终结符

%type <number> type;
%type <condition1> condition;
%type <value1> value;
%type <number> number;
%type <bools> nullable;

%%

commands:		//commands or sqls. parser starts here.
    /* empty */
    | commands command
    ;
command:
	q SEMICOLON;

q:
	  select
	| insert
	| update
	| delete
	| create_table
	| drop_table
	| show_tables
	| desc_table
	| create_index
	| drop_index
	| sync
	| begin
	| commit
	| rollback
	| load_data
	| help
	| exit
    ;

exit:
    EXIT  {
        CONTEXT->ssql->flag=SCF_EXIT;//"exit";
    };

help:
    HELP  {
        CONTEXT->ssql->flag=SCF_HELP;//"help";
    };

sync:
    SYNC  {
      CONTEXT->ssql->flag = SCF_SYNC;
    }
    ;

begin:
    TRX_BEGIN  {
      CONTEXT->ssql->flag = SCF_BEGIN;
    }
    ;

commit:
    TRX_COMMIT  {
      CONTEXT->ssql->flag = SCF_COMMIT;
    }
    ;

rollback:
    TRX_ROLLBACK  {
      CONTEXT->ssql->flag = SCF_ROLLBACK;
    }
    ;

drop_table:		/*drop table 语句的语法解析树*/
    DROP TABLE ID  {
        CONTEXT->ssql->flag = SCF_DROP_TABLE;//"drop_table";
        drop_table_init(&CONTEXT->ssql->sstr.drop_table, $3);
    };

show_tables:
    SHOW TABLES  {
      CONTEXT->ssql->flag = SCF_SHOW_TABLES;
    }
    ;

desc_table:
    DESC ID  {
      CONTEXT->ssql->flag = SCF_DESC_TABLE;
      desc_table_init(&CONTEXT->ssql->sstr.desc_table, $2);
    }
    ;

create_index:		/*create index 语句的语法解析树*/
    CREATE INDEX ID ON ID LBRACE ID col_list RBRACE
		{
			CONTEXT->ssql->flag = SCF_CREATE_INDEX;//"create_index";
			create_index_init(&CONTEXT->ssql->sstr.create_index, $3, $5, $7, false);
		}
    | CREATE UNIQUE INDEX ID ON ID LBRACE ID RBRACE
    		{
			CONTEXT->ssql->flag = SCF_CREATE_INDEX;//"create_index";
			create_index_init(&CONTEXT->ssql->sstr.create_index, $4, $6, $8, true);
    		}
    ;
col_list:
	| COMMA ID col_list {
		append_index(&CONTEXT->ssql->sstr.create_index, $2);
	}

drop_index:			/*drop index 语句的语法解析树*/
    DROP INDEX ID
		{
			CONTEXT->ssql->flag=SCF_DROP_INDEX;//"drop_index";
			drop_index_init(&CONTEXT->ssql->sstr.drop_index, $3);
		}
    ;
create_table:		/*create table 语句的语法解析树*/
    CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE
		{
			CONTEXT->ssql->flag=SCF_CREATE_TABLE;//"create_table";
			create_table_init_name(&CONTEXT->ssql->sstr.create_table, $3);
			//临时变量清零
			CONTEXT->value_length = 0;
		}
    ;
attr_def_list:
    /* empty */
    | COMMA attr_def attr_def_list {    }
    ;

attr_def:
    ID_get type LBRACE number RBRACE nullable
		{
			AttrInfo attribute;
			attr_info_init(&attribute, CONTEXT->id, $2, $4, $6);
			create_table_append_attribute(&CONTEXT->ssql->sstr.create_table, &attribute);
			CONTEXT->value_length++;
		}
    |ID_get type nullable
		{
			AttrInfo attribute;
			// 从 1970-01-01 ~ 2038-03-01 有 24896 天，DATE 类型使用两个字节存储
			attr_info_init(&attribute, CONTEXT->id, $2, $2 == DATES ? 2 : 4, $3);
			create_table_append_attribute(&CONTEXT->ssql->sstr.create_table, &attribute);
			CONTEXT->value_length++;
		}
    ;

nullable:
    { $$ = false; }
    | NULLABLE { $$ = true; }
    | NOT NULL_TOK { $$ = false; }
    ;

number:
		NUMBER {$$ = $1;}
		;
type:
	INT_T { $$=INTS; }
       | DATE_T { $$=DATES; }
       | STRING_T { $$=CHARS; }
       | FLOAT_T { $$=FLOATS; }
       | TEXT_T { $$=TEXTS; }
       ;
ID_get:
	ID
	{
		char *temp=$1;
		snprintf(CONTEXT->id, sizeof(CONTEXT->id), "%s", temp);
	}
	;

insert:				/*insert   语句的语法解析树*/
    INSERT INTO ID VALUES tuple tuple_list
		{
			CONTEXT->ssql->flag=SCF_INSERT;//"insert";
			inserts_init(&CONTEXT->ssql->sstr.insertion, $3, CONTEXT->tuples, CONTEXT->tuple_length);

      //临时变量清零
      CONTEXT->value_length=0;
      CONTEXT->tuple_length=0;
      memset(CONTEXT->tuples, 0, sizeof(CONTEXT->tuples));
    }

tuple_list:
    | COMMA tuple tuple_list {
    }
    ;

tuple:
    | LBRACE value value_list RBRACE {
    	memcpy(CONTEXT->tuples[CONTEXT->tuple_length].values, CONTEXT->values, sizeof(Value)*CONTEXT->value_length);
    	CONTEXT->tuples[CONTEXT->tuple_length].value_num = CONTEXT->value_length;
    	CONTEXT->value_length = 0;
        CONTEXT->tuple_length++;
    }
    ;

value_list:
    /* empty */
    | COMMA value value_list  { }
    ;
value:
    NUMBER{
  		value_init_integer(&CONTEXT->values[CONTEXT->value_length++], $1);
		}
    |FLOAT{
  		value_init_float(&CONTEXT->values[CONTEXT->value_length++], $1);
		}
    |SSS {
		$1 = substr($1,1,strlen($1)-2);
  		value_init_string(&CONTEXT->values[CONTEXT->value_length++], $1);
		}
    |NULL_TOK {
    		value_init_null(&CONTEXT->values[CONTEXT->value_length++]);
    }
    ;

delete:		/*  delete 语句的语法解析树*/
    DELETE FROM ID where
		{
			CONTEXT->ssql->flag = SCF_DELETE;//"delete";
			deletes_init_relation(&CONTEXT->ssql->sstr.deletion, $3);
			deletes_set_conditions(&CONTEXT->ssql->sstr.deletion,
					CONTEXT->conditions, CONTEXT->condition_length);
			CONTEXT->condition_length = 0;
    }
    ;
update:			/*  update 语句的语法解析树*/
    UPDATE ID SET ID EQ value where
		{
			CONTEXT->ssql->flag = SCF_UPDATE;//"update";
			Value *value = &CONTEXT->values[0];
			updates_init(&CONTEXT->ssql->sstr.update, $2, $4, value,
					CONTEXT->conditions, CONTEXT->condition_length);
			CONTEXT->value_length = 0;
			CONTEXT->condition_length = 0;
		}
    ;
select:				/*  select 语句的语法解析树*/
    begin_select SELECT select_attr FROM ID rel_list where groupbys orderbys
		{
			selects_append_relation(&CONTEXT->ssql->sstr.selection, $5);

			selects_append_conditions(&CONTEXT->ssql->sstr.selection,
				CONTEXT->conditions,
				CONTEXT->last_condition_length[CONTEXT->last_condition_length_length - 1],
				CONTEXT->condition_length);
			CONTEXT->ssql->flag=SCF_SELECT;//"select";
	}
	;

begin_select: {
	if (CONTEXT->last_condition_length_length != 0) {
		CONTEXT->queries[CONTEXT->query_length++] = CONTEXT->ssql;
		CONTEXT->ssql = query_create();
	}
	CONTEXT->last_condition_length[CONTEXT->last_condition_length_length++] = CONTEXT->condition_length;
}
;

select_attr:
    STAR {
			RelAttr attr;
			relation_attr_init(&attr, NULL, "*");
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
		}
    | ID attr_list {
			RelAttr attr;
			relation_attr_init(&attr, NULL, $1);
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
		}
  	| ID DOT ID attr_list {
			RelAttr attr;
			relation_attr_init(&attr, $1, $3);
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
		}
	| ID DOT STAR attr_list {
		RelAttr attr;
		relation_attr_init(&attr, $1, "*");
		selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
	}
	| func LBRACE expression RBRACE attr_list {
	}
    ;
expression:
	STAR {// *
			RelAttr attr;
			relation_attr_init(&attr, NULL, "*");
			Aggregation aggr;
			aggr.attribute = attr;
			aggr.func_name = CONTEXT->func;
			aggr.is_value = 0;
			selects_append_aggregation(&CONTEXT->ssql->sstr.selection,&aggr);
	}
	| ID{// age
			RelAttr attr;
			relation_attr_init(&attr, NULL, $1);
			Aggregation aggr;
			aggr.attribute = attr;
			aggr.func_name = CONTEXT->func;
			aggr.is_value = 0;
			selects_append_aggregation(&CONTEXT->ssql->sstr.selection,&aggr);
	}
	| ID DOT ID{// age
			RelAttr attr;
			relation_attr_init(&attr, $1, $3);
			Aggregation aggr;
			aggr.attribute = attr;
			aggr.func_name = CONTEXT->func;
			aggr.is_value = 0;
			selects_append_aggregation(&CONTEXT->ssql->sstr.selection,&aggr);
	}
	| value{ // 1
			Aggregation aggr;
			aggr.func_name = CONTEXT->func;
			aggr.is_value = 1;
			aggr.value = CONTEXT->values[CONTEXT->value_length - 1];
			CONTEXT->value_length = 0;
			selects_append_aggregation(&CONTEXT->ssql->sstr.selection,&aggr);
	}

attr_list:
    /* empty */
    | COMMA ID attr_list {
			RelAttr attr;
			relation_attr_init(&attr, NULL, $2);
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
      }
    | COMMA ID DOT ID attr_list {
			RelAttr attr;
			relation_attr_init(&attr, $2, $4);
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
  	  }
	| COMMA ID DOT STAR attr_list {
		RelAttr attr;
		relation_attr_init(&attr, $2, "*");
		selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
	}
	| COMMA func LBRACE expression RBRACE attr_list{

	}
  	;

rel_list:
    	/* empty */
    	| COMMA ID rel_list {
			selects_append_relation(&CONTEXT->ssql->sstr.selection, $2);
	}
	| INNER JOIN ID ON condition condition_list rel_list
	{
		selects_append_relation(&CONTEXT->ssql->sstr.selection, $3);
	}
    ;
where:
    /* empty */
    | WHERE condition condition_list {
			}
    ;
condition_list:
    /* empty */
    | AND condition condition_list {
			}
    ;
condition:
                ID IS NULL_TOK
                {
			RelAttr left_attr;
			relation_attr_init(&left_attr, NULL, $1);

			Condition condition;
			condition_init(&condition, IS_LEFT_NULL, 1, &left_attr, NULL, 0, 0, NULL, NULL, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
                }
                | ID IS NOT NULL_TOK
                {
			RelAttr left_attr;
			relation_attr_init(&left_attr, NULL, $1);

			Condition condition;
			condition_init(&condition, IS_LEFT_NOT_NULL, 1, &left_attr, NULL, 0, 0, NULL, NULL, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
                }
                | ID DOT ID IS NULL_TOK
                {
			RelAttr left_attr;
			relation_attr_init(&left_attr, $1, $3);

			Condition condition;
			condition_init(&condition, IS_LEFT_NULL, 1, &left_attr, NULL, 0, 0, NULL, NULL, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
                }
                | ID DOT ID IS NOT NULL_TOK
                {
			RelAttr left_attr;
			relation_attr_init(&left_attr, NULL, $1);

			Condition condition;
			condition_init(&condition, IS_LEFT_NOT_NULL, 1, &left_attr, NULL, 0, 0, NULL, NULL, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
                }
                | ID comOp value
		{
			RelAttr left_attr;
			relation_attr_init(&left_attr, NULL, $1);

			Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];
			CONTEXT->value_length = 0;

			Condition condition;
			condition_init(&condition, CONTEXT->comp, 1, &left_attr, NULL, 0, 0, NULL, NULL, right_value);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
		}
		| value comOp value
		{
			Value *left_value = &CONTEXT->values[CONTEXT->value_length - 2];
			Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];
			CONTEXT->value_length = 0;

			Condition condition;
			condition_init(&condition, CONTEXT->comp, 0, NULL, left_value, 0, 0, NULL, NULL, right_value);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
		}
		| value IS NULL_TOK
		{
			Value *left_value = &CONTEXT->values[CONTEXT->value_length - 1];
			CONTEXT->value_length = 0;

			Condition condition;
			condition_init(&condition, IS_LEFT_NULL, 0, NULL, left_value, 0, 0, NULL, NULL, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
		}
		| value IS NOT NULL_TOK
		{
			Value *left_value = &CONTEXT->values[CONTEXT->value_length - 1];
			CONTEXT->value_length = 0;

			Condition condition;
			condition_init(&condition, IS_LEFT_NOT_NULL, 0, NULL, left_value, 0, 0, NULL, NULL, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
		}
		| ID comOp ID
		{
			RelAttr left_attr;
			relation_attr_init(&left_attr, NULL, $1);
			RelAttr right_attr;
			relation_attr_init(&right_attr, NULL, $3);

			Condition condition;
			condition_init(&condition, CONTEXT->comp, 1, &left_attr, NULL, 0, 1, NULL, &right_attr, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
		}
                | value comOp ID
		{
			fprintf(stderr, "Enter");
			Value *left_value = &CONTEXT->values[CONTEXT->value_length - 1];
			CONTEXT->value_length = 0;

			RelAttr right_attr;
			relation_attr_init(&right_attr, NULL, $3);

			Condition condition;
			condition_init(&condition, CONTEXT->comp, 0, NULL, left_value, 0, 1, NULL, &right_attr, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;

		}
                | ID DOT ID comOp value
		{
			fprintf(stderr, "Enter");
			RelAttr left_attr;
			relation_attr_init(&left_attr, $1, $3);
			Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];
			CONTEXT->value_length = 0;

			Condition condition;
			condition_init(&condition, CONTEXT->comp, 1, &left_attr, NULL, 0, 0, NULL, NULL, right_value);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
                }
                | value comOp ID DOT ID
		{
			Value *left_value = &CONTEXT->values[CONTEXT->value_length - 1];
			CONTEXT->value_length = 0;

			RelAttr right_attr;
			relation_attr_init(&right_attr, $3, $5);

			Condition condition;
			condition_init(&condition, CONTEXT->comp, 0, NULL, left_value, 0, 1, NULL, &right_attr, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
                }
                |ID DOT ID comOp ID DOT ID
		{
			RelAttr left_attr;
			relation_attr_init(&left_attr, $1, $3);
			RelAttr right_attr;
			relation_attr_init(&right_attr, $5, $7);

			Condition condition;
			condition_init(&condition, CONTEXT->comp, 1, &left_attr, NULL, 0, 1, NULL, &right_attr, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
                }
                | ID comOp LBRACE select RBRACE
                {
			RelAttr left_attr;
			relation_attr_init(&left_attr, NULL, $1);

			Condition condition;
			condition_init(&condition, CONTEXT->comp, 1, &left_attr, NULL, 1, 0, &CONTEXT->ssql->sstr.selection, NULL, NULL);
			CONTEXT->ssql = CONTEXT->queries[CONTEXT->query_length - 1];
			CONTEXT->query_length--;
			CONTEXT->condition_length = CONTEXT->last_condition_length[CONTEXT->last_condition_length_length - 1];
			CONTEXT->last_condition_length_length--;
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;

                }
                | ID DOT ID comOp LBRACE select RBRACE
                {
			RelAttr left_attr;
			relation_attr_init(&left_attr, $1, $3);

			Condition condition;
			condition_init(&condition, CONTEXT->comp, 1, &left_attr, NULL, 1, 0, &CONTEXT->ssql->sstr.selection, NULL, NULL);
			CONTEXT->ssql = CONTEXT->queries[CONTEXT->query_length - 1];
			CONTEXT->query_length--;
			CONTEXT->condition_length = CONTEXT->last_condition_length[CONTEXT->last_condition_length_length - 1];
			CONTEXT->last_condition_length_length--;
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
                }
    		;

comOp:
  	  EQ { CONTEXT->comp = EQUAL_TO; }
    | LT { CONTEXT->comp = LESS_THAN; }
    | GT { CONTEXT->comp = GREAT_THAN; }
    | LE { CONTEXT->comp = LESS_EQUAL; }
    | GE { CONTEXT->comp = GREAT_EQUAL; }
    | NE { CONTEXT->comp = NOT_EQUAL; }
    | IN { CONTEXT->comp = IN_COMP; }
    | NOT IN { CONTEXT->comp = NOT_IN_COMP; }
    ;

func:
  	  AGGMAX { CONTEXT->func = AGG_MAX; }
    | AGGMIN { CONTEXT->func = AGG_MIN; }
    | AGGCOUNT { CONTEXT->func = AGG_COUNT; }
    | AGGAVG { CONTEXT->func = AGG_AVG; }
    ;

groupbys:
	| GROUP BY ID DOT ID groupby{
		RelAttr attr;
		relation_attr_init(&attr, $3, $5);
		selects_append_group_by(&CONTEXT->ssql->sstr.selection, &attr);
	}
	| GROUP BY ID groupby{
		RelAttr attr;
		relation_attr_init(&attr, NULL, $3);
		selects_append_group_by(&CONTEXT->ssql->sstr.selection, &attr);
	}
groupby:
	| COMMA ID DOT ID groupby {
		RelAttr attr;
		relation_attr_init(&attr, $2, $4);
		selects_append_group_by(&CONTEXT->ssql->sstr.selection, &attr);
	}
	| COMMA ID groupby {
		RelAttr attr;
		relation_attr_init(&attr, NULL,$2);
		selects_append_group_by(&CONTEXT->ssql->sstr.selection, &attr);
	}
	;
orderbys:
	| ORDER BY ID orderby {
		RelAttr attr;
		relation_attr_init(&attr, NULL, $3);

		OrderBy order_by;
		order_by_init(&order_by, &attr, true);
		selects_append_order_by(&CONTEXT->ssql->sstr.selection, &order_by);
	}
	| ORDER BY ID ASC orderby {
        	RelAttr attr;
		relation_attr_init(&attr, NULL, $3);

		OrderBy order_by;
		order_by_init(&order_by, &attr, true);
		selects_append_order_by(&CONTEXT->ssql->sstr.selection, &order_by);
        }
	| ORDER BY ID DESC orderby {
		RelAttr attr;
		relation_attr_init(&attr, NULL, $3);

		OrderBy order_by;
		order_by_init(&order_by, &attr, false);
		selects_append_order_by(&CONTEXT->ssql->sstr.selection, &order_by);
	}
	| ORDER BY ID DOT ID ASC orderby{
		RelAttr attr;
		relation_attr_init(&attr, $3, $5);

		OrderBy order_by;
		order_by_init(&order_by, &attr, true);
		selects_append_order_by(&CONTEXT->ssql->sstr.selection, &order_by);
	}
	| ORDER BY ID DOT ID orderby{
		RelAttr attr;
		relation_attr_init(&attr, $3, $5);

		OrderBy order_by;
		order_by_init(&order_by, &attr, true);
		selects_append_order_by(&CONTEXT->ssql->sstr.selection, &order_by);
	}
	| ORDER BY ID DOT ID DESC orderby{
		RelAttr attr;
		relation_attr_init(&attr, $3, $5);

		OrderBy order_by;
		order_by_init(&order_by, &attr, false);
		selects_append_order_by(&CONTEXT->ssql->sstr.selection, &order_by);
	}
orderby:
	| COMMA ID orderby{
		RelAttr attr;
		relation_attr_init(&attr, NULL, $2);

		OrderBy order_by;
		order_by_init(&order_by, &attr, true);
		selects_append_order_by(&CONTEXT->ssql->sstr.selection, &order_by);
	}
	| COMMA ID ASC orderby{
		RelAttr attr;
		relation_attr_init(&attr, NULL, $2);

		OrderBy order_by;
		order_by_init(&order_by, &attr, true);
		selects_append_order_by(&CONTEXT->ssql->sstr.selection, &order_by);
	}
	| COMMA ID DESC orderby {
		RelAttr attr;
		relation_attr_init(&attr, NULL, $2);

		OrderBy order_by;
		order_by_init(&order_by, &attr, false);
		selects_append_order_by(&CONTEXT->ssql->sstr.selection, &order_by);
	}
	| COMMA ID DOT ID ASC orderby{
		RelAttr attr;
		relation_attr_init(&attr, $2, $4);

		OrderBy order_by;
		order_by_init(&order_by, &attr, true);
		selects_append_order_by(&CONTEXT->ssql->sstr.selection, &order_by);
	}
	| COMMA ID DOT ID orderby{
		RelAttr attr;
		relation_attr_init(&attr, $2, $4);

		OrderBy order_by;
		order_by_init(&order_by, &attr, true);
		selects_append_order_by(&CONTEXT->ssql->sstr.selection, &order_by);
	}
	| COMMA ID DOT ID DESC orderby{
		RelAttr attr;
		relation_attr_init(&attr, $2, $4);

		OrderBy order_by;
		order_by_init(&order_by, &attr, false);
		selects_append_order_by(&CONTEXT->ssql->sstr.selection, &order_by);
	}
load_data:
		LOAD DATA INFILE SSS INTO TABLE ID
		{
		  CONTEXT->ssql->flag = SCF_LOAD_DATA;
			load_data_init(&CONTEXT->ssql->sstr.load_data, $7, $4);
		}
		;
%%
//_____________________________________________________________________
extern void scan_string(const char *str, yyscan_t scanner);

int sql_parse(const char *s, Query *sqls){
	ParserContext context;
	memset(&context, 0, sizeof(context));

	yyscan_t scanner;
	yylex_init_extra(&context, &scanner);
	context.ssql = sqls;
	scan_string(s, scanner);
	int result = yyparse(scanner);
	yylex_destroy(scanner);
	return result;
}
