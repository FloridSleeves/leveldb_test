#include <iostream>
#include <cassert>
#include <string>
#include <cstdlib>
#include "leveldb/db.h"
// #include "leveldb/filter_policy.h"
#define KEY_LIMIT 1000000
#define READ_LIMIT 500
using namespace std;

int main()
{
    string count;
    leveldb::DB* db;
    leveldb::Options options;
    options.create_if_missing=true;
    options.error_if_exists=false;
    //options.filter_policy=leveldb::NewBloomFilterPolicy(10);
    leveldb::Status status=leveldb::DB::Open(options, "./testdb", &db);
    assert(status.ok());
    printf("Load...\n");
    for(int i=0;i<=KEY_LIMIT;i+=1)
    {
        string value=to_string(i);
        string key=to_string(i);
        status=db->Put(leveldb::WriteOptions(),key,value);
        assert(status.ok());
    }
    string value;
    printf("Test read...\n");
    int count_read_error=0;
    for(int i=0;i<=KEY_LIMIT;i+=1)
    {
        int random_num=rand()%KEY_LIMIT;
        // int random_num = i;
        string value=to_string(random_num);
        string key=to_string(random_num);
        string readvalue;
        status=db->Get(leveldb::ReadOptions(),key,&readvalue);
        if(value.compare(readvalue)!=0){
            count_read_error++;
            printf("Error! Value mismatch! Found %s but should be %s\n", readvalue.c_str(), value.c_str());
        }
        if(!status.ok()){
          printf("Error! Key %d not found!\n", random_num);
        }
    }
    if(count_read_error==0)
        printf("Read test completed, all read succeeded!\n");
    else
        printf("Read test error: %d out of %d\n",count_read_error, KEY_LIMIT);

    printf("Update...\n");

    for(int i=0;i<KEY_LIMIT;i+=5)
    {
        string value="new"+to_string(i);
        string key=to_string(i);
        status=db->Put(leveldb::WriteOptions(),key,value);
    }

    printf("Test read...\n");
    count_read_error=0;
    for(int i=0;i<=READ_LIMIT;i+=1)
    {
        int random_num=rand()%KEY_LIMIT;
        string value=to_string(random_num);
        if (random_num%5==0)
            value="new"+to_string(random_num);
        string key=to_string(random_num);
        string readvalue;
        status=db->Get(leveldb::ReadOptions(),key,&readvalue);
        if(value.compare(readvalue)!=0){
            printf("Error! Value mismatch! Found %s but should be %s\n", readvalue.c_str(), value.c_str());
            count_read_error++;
        }
    }
    if(count_read_error==0)
        printf("Read test completed\n");
    else
        printf("Read test error: %d\n",count_read_error);

    delete db;
}
