
#include <stdlib.h>
#include <string.h>

#include <iomanip>
#include <iostream>
#include <string>
#include <sys/types.h>

#include "db.h"
#include "db_cxx.h"

using namespace std;

const char *kDatabaseName = "access.db";

void LogBDBError(const DbEnv *, const char * /*errpfx*/, const char *msg) {
  std::cout << "[bdb] error msg: " << msg << "\n";
}

int main() {

  string fruit("fruit");
  string apple("apple");
  string orange("orange");

  DbEnv env(0);
  Db *db;

  // Env open flags
  uint32_t env_flags =
      DB_CREATE |       // Create the environment if it does not exist
      DB_RECOVER |      // Run normal recovery.
      DB_INIT_LOCK |    // Initialize the locking subsystem
      DB_INIT_LOG |     // Initialize the logging subsystem
      DB_INIT_TXN |     // Initialize the transactional subsystem. This
                        // also turns on logging.
      DB_INIT_MPOOL |   // Initialize the memory pool (in-memory cache)
      DB_MULTIVERSION | // Multiversion concurrency control
      //  DB_PRIVATE |  // Region files are not backed by the filesystem, but
      //  instead are backed by heap
      // memory. This flag is required for the in-memory cache.
      DB_THREAD; // Cause the environment to be free-threaded

  try {
    env.set_error_stream(&cerr);
    // Indicate that we want db to internally perform deadlock
    // detection.  Also indicate that the transaction with
    // the fewest number of write locks will receive the
    // deadlock notification in the event of a deadlock.
    env.set_lk_detect(DB_LOCK_MINWRITE);
    // env.set_lk_max_lockers(100);
    // env.set_lk_max_locks(100);
    // env.set_lk_max_objects(100);

    env.set_cachesize(4, 0, 1);

    // shared mem
    // env.set_memory_max(4, 0);
    // env.set_memory_init(DB_MEM_LOCK, 1000000000);
    // env.set_memory_init(DB_MEM_LOCKOBJECT, 1000000000);
    // env.set_memory_init(DB_MEM_LOGID, 1000000000);
    // env.set_memory_init(DB_MEM_TRANSACTION, 1000000000);
    // env.set_memory_init(DB_MEM_THREAD, 1000000000);

    // mutex region size
    // max = __lock_region_mutex_max(env) + __txn_region_mutex_max(env) +  __log_region_mutex_max(env) + dbenv->mutex_inc + 100;
    env.mutex_set_increment(10000);

    // DB_MEM_LOCK=1,
    // DB_MEM_LOCKOBJECT=2,
    // DB_MEM_LOCKER=3,
    // DB_MEM_LOGID=4,
    // DB_MEM_TRANSACTION=5,
    // DB_MEM_THREAD=6

    // __env_set_max

    // set error call back
    env.set_errcall(LogBDBError);

    // print txn_max
    uint32_t txn_max = 0;
    env.get_tx_max(&txn_max);

    std::cout << "[bdb] default txn_max:" << txn_max << std::endl;

    env.set_tx_max(100);
    env.get_tx_max(&txn_max);

    // set lock timeout to 5s, the first parameter is microsecond
    env.set_timeout(5 * 1000 * 1000, DB_SET_LOCK_TIMEOUT);

    std::cout << "[bdb] set txn_max to:" << txn_max << std::endl;
    env.open("/root/db", env_flags, 0);

    db = new Db(&env, 0);
    // If you want to support duplicated records and make duplicated
    // records sorted by data, you need to call:
    //   db->set_flags(DB_DUPSORT);
    // Note that only Btree-typed database supports sorted duplicated
    // records

    // If the database does not exist, create it.  If it exists, clear
    // its content after openn    // Now open the database */
    u_int32_t open_flags =
        DB_CREATE |       // Allow database creation
                          //  DB_READ_UNCOMMITTED |  // Allow uncommitted reads
        DB_AUTO_COMMIT |  // Allow autocommit
        DB_MULTIVERSION | // Multiversion concurrency control
        DB_THREAD;        // Cause the database to be free-threade1ing.
    db->open(NULL, "access.db", NULL, DB_BTREE, open_flags, 0);

    Dbt key(const_cast<char *>(fruit.data()), fruit.size());
    Dbt value(const_cast<char *>(apple.data()), apple.size() + 1);
    db->put(NULL, &key, &value, 0);

    Dbt value_orange(const_cast<char *>(orange.data()), orange.size() + 1);
    db->put(NULL, &key, &value_orange, 0);

    // You need to set ulen and flags=DB_DBT_USERMEM to prevent Dbt
    // from allocate its own memory but use the memory provided by you.
    char buffer[1024];
    Dbt data;
    data.set_data(buffer);
    data.set_ulen(1024);
    data.set_flags(DB_DBT_USERMEM);
    if (db->get(NULL, &key, &data, 0) == DB_NOTFOUND) {
      cerr << "Not found" << endl;
    } else {
      cout << "Found: " << buffer << endl;
    }

    {
      std::string apple("apple");
      std::string orange("orange");
      for (int i = 0; i < 100000; i++) {
        std::string fruit("fruit" + std::to_string(i));

        Dbt key(const_cast<char *>(fruit.data()), fruit.size());
        Dbt value(const_cast<char *>(apple.data()), apple.size() + 1);
        db->put(nullptr, &key, &value, 0);

        char buffer[1024];
        Dbt data;
        data.set_data(buffer);
        data.set_ulen(1024);
        data.set_flags(DB_DBT_USERMEM);

        if (db->get(nullptr, &key, &data, 0) == DB_NOTFOUND) {
          std::cerr << "Not found" << std::endl;
        }
      }

      {
        for (int i = 0; i < 10; i++) {
          DbTxn *read = nullptr;
          int ret = env.txn_begin(nullptr, &read, DB_TXN_SNAPSHOT);
          //   int ret = envp->txn_begin(nullptr, &read, 0);
          if (ret != 0) {
            std::cerr << "[bdb] txn begin failed ret:" << ret << "\n";
            return ret;
          }

          // It seems bdb do a snapshot after a cursor with the txn do its first
          // get. So we need to init a cursor and do a get to make sure the
          // snapshot is valid. Acquire a cursor
          Dbc *cursorp = nullptr;
          ret = db->cursor(read, &cursorp, DB_TXN_SNAPSHOT);
          //   ret = db->cursor(read, &cursorp, 0);
          if (ret != 0) {
            std::cerr << "[bdb] cursor failed ret: " << ret << "\n";
            return ret;
          }

          char buffer[1024];
          Dbt data;
          data.set_data(buffer);
          data.set_ulen(1024);
          data.set_flags(DB_DBT_USERMEM);

          std::string fruit("fruit" + std::to_string(i));
          Dbt key(const_cast<char *>(fruit.data()), fruit.size());
          if (db->get(read, &key, &data, 0) == DB_NOTFOUND) {
            std::cerr << "read txn Not found" << std::endl;
          } else {
            std::cout << "read txn Found: " << buffer << std::endl;
          }

          cursorp->close();
          cursorp = nullptr;

          read->commit(0);
          read = nullptr;

          db->sync(0);
          return 0;
        }
      }
    }

    {
      if (db != NULL) {
        db->close(0);
        delete db;
        // You have to close and delete an exisiting handle, then create
        // a new one before you can use it to remove a database (file).
        db = new Db(NULL, 0);
        db->remove("/root/db/access.db", NULL, 0);
        delete db;
      }
      env.close(0);
    }
  } catch (DbException &e) {
    cerr << "DbException: " << e.what() << endl;
    return -1;
  } catch (std::exception &e) {
    cerr << e.what() << endl;
    return -1;
  }

  return 0;
}
