#ifndef __OWNSQL_H__
#define __OWNSQL_H__

#include <errno.h>
#include <mysql/mysql.h>
#include <string>
#include <sstream>
#include <iostream>
#include "RandomNumberCreator.h"

using std::perror;
using std::string;
using std::stringstream;

static bool isConnected = false;

static void IsConnectedDatabase(){
    if(!isConnected){
        perror("connect database first\n");
    }
}

template<typename T>
static string ToStr(T&& num)
{
    stringstream ss;

    ss.clear();
    ss << num;

    return ss.str();
}

void MysqlConnect(MYSQL* mysql,const char* host = "localhost",const char* user = "word",const char* password = "",const char* database = "dictionary",unsigned int port = 3306,const char* unix_socket = nullptr,unsigned long clientflag = 0){
    if(!mysql_real_connect(mysql,host,user,password,database,port,unix_socket,clientflag)){
        perror("connect failed!\n");

        return;
    }
    
    isConnected = true;
}

class sqlbase{
    protected:
        MYSQL* ptr = nullptr;
        MYSQL_ROW row = nullptr;
        MYSQL_RES* result = nullptr;
        int ct = 0;
        
        explicit sqlbase(){
            ptr = new MYSQL;
            mysql_init(ptr);

            MysqlConnect(ptr);

            mysql_query(ptr,"SELECT COUNT(*) FROM dict;");

            result = mysql_store_result(ptr);

            if(!result){
                perror("count failed\n");

                return;
            }

            row = mysql_fetch_row(result);

            ct = atoi(row[0]);
        }

        virtual ~sqlbase(){
            delete ptr;
            std::cout << "sqlbase delete\n";
        }
    public:
        virtual void request(char) = 0;
        
        void request(std::string&&) = delete;
        void request(std::string) = delete;
};

class sqltrue:public sqlbase{
    public:
        sqltrue():sqlbase(){}
        ~sqltrue(){
            std::cout << "sqltrue delete\n";
        }

        void request(char choice){
            IsConnectedDatabase();

            switch(choice){
                case 'w':
                    select();
                    
                    break;
                case 'i':
                    insert();

                    break;
                case 'd':
                    drop();

                    break;
                default:
                    perror("use w or i or d\n");
                    
                    return;
            }
        }
    private:
        void insert(){
            ++ct;

            string ch,en;

            printf("input ch and en:");
            std::cin >> ch >> en;

            string table = "INSERT INTO dict (ch,en)values(\""+ch+"\",\""+en+"\");";
        
            if(mysql_query(ptr,table.c_str())){
                perror("insert fault!\n");
            }
        }

        void drop(){
            if(ct==0){
                perror("the table is empty\n");

                return;
            }

            string table;

            printf("input ch and en:");

            string ch,en;

            std::cin >> ch >> en;

            table = "DELETE FROM dict WHERE (ch=\""+ch+"\" AND en=\""+en+"\") LIMIT 1;";

            if(mysql_query(ptr,table.c_str())){
                perror("acquire fields list fault!\n");
            }

            --ct;
        }

        void select(){
            if(ct==0){
                std::cerr << "insert a word first\n";
            }
            else{
                int randNumber = RandomNumberCreator(1,(int)ct);

                string table = "SELECT * FROM dict LIMIT "+ToStr(randNumber-1)+",1";
                
                if(mysql_query(ptr,table.c_str())){
                    perror("acquire fields list fault!\n");
                }
                
                result = mysql_store_result(ptr);
                
                if(!result){
                    printf("result fault!\n");

                    return;
                }

                row = mysql_fetch_row(result);

                printf("%s\t\t%s\n",row[0],row[1]);

                result = nullptr;
                row = nullptr;
            }
        }   
};


#endif
