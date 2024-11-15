MESSAGE(STATUS "This is CMAKE_CURRENT_SOURCE_DIR dir " ${CMAKE_CURRENT_SOURCE_DIR})

# 添加本目录到头文件目录
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

# 添加源文件
FILE(GLOB_RECURSE ALL_SRC 
    Main.cpp
    HttpServer.cpp 
    Config.cpp
    ClientAuthList.cpp
    Logger.cpp
    # Bitcask
    KV/Bitcask/BitcaskImpl.cpp
    KV/File/PosixFile.cpp
    KV/Util/Arena.cpp
    KV/Util/Coding.cpp
    # fandb
    fandb/buffer-manager/AsyncWriteBuffer.cpp
    # MVCC
    # KV/Transaction/LockManager.cpp
    # KV/Transaction/MVCCStorage.cpp
    # KV/Transaction/Storage.cpp
    # KV/Transaction/Txn.cpp
    # KV/Transaction/TxnProcessor.cpp
    # 参考duckdb
    # KV/Transaction/cleanup_state.cpp
    # KV/Transaction/commit_state.cpp
    # KV/Transaction/duck_transaction.cpp
    # KV/Transaction/duck_transaction_manager.cpp
    # KV/Transaction/meta_transaction.cpp
    # KV/Transaction/rollback_state.cpp
    # KV/Transaction/transaction.cpp
    # KV/Transaction/transaction_context.cpp
    # KV/Transaction/transaction_manager.cpp
    # KV/Transaction/undo_buffer.cpp
    # 参考hyrise
    # KV/Transaction/commit_context.cpp
    # KV/Transaction/transaction_context.cpp
    # KV/Transaction/transaction_manager.cpp
    # KV/Transaction/types.cpp
    # KV/Transaction/Storage/base_attribute_vector.hpp
    # KV/Transaction/Storage/chunk.cpp
    # KV/Transaction/Storage/reference_column.cpp
    # KV/Transaction/Storage/storage_manager.cpp
    # KV/Transaction/Storage/table.cpp
    # KV/Transaction/Utils/base_test.cpp

    hyrise/types.cpp 
    hyrise/concurrency/Utils/base_test.cpp
    hyrise/concurrency/commit_context.cpp 
    hyrise/concurrency/transaction_context.cpp 
    hyrise/concurrency/transaction_manager.cpp 
    hyrise/operators/abstract_operator.cpp 
    hyrise/operators/get_table.cpp 
    hyrise/operators/print.cpp 
    hyrise/operators/table_scan.cpp 
    hyrise/storage/chunk.cpp 
    hyrise/storage/reference_column.cpp 
    hyrise/storage/storage_manager.cpp 
    hyrise/storage/table.cpp 
    hyrise/scheduler/job_task.cpp
    hyrise/scheduler/node_queue_scheduler.cpp
    hyrise/scheduler/operator_task.cpp
    hyrise/scheduler/processing_unit.cpp
    hyrise/scheduler/task_queue.cpp
    hyrise/scheduler/topology.cpp
    hyrise/scheduler/worker.cpp
    hyrise/scheduler/abstract_scheduler.cpp
    hyrise/scheduler/abstract_task.cpp
    hyrise/scheduler/current_scheduler.cpp
    hyrise/benchmark/base_fixture.cpp
    hyrise/benchmark/table_generator.cpp

    # leanstore
    # leanstore/LeanStore.cpp
    # leanstore/utils/Misc.cpp
    # leanstore/utils/Parallelize.cpp
    # leanstore/utils/RandomGenerator.cpp
    # leanstore/utils/ScrambledZipfGenerator.cpp
    # leanstore/utils/ZipfGenerator.cpp
    # leanstore/utils/FNVHash.cpp
    # leanstore/utils/JumpMU.cpp
    # leanstore/utils/Log.cpp
    # leanstore/buffer-manager/Partition.cpp
    # leanstore/buffer-manager/AsyncWriteBuffer.cpp
    # leanstore/buffer-manager/BufferManager.cpp
    # leanstore/buffer-manager/PageEvictor.cpp
    # leanstore/concurrency/ConcurrencyControl.cpp
    # leanstore/concurrency/CRManager.cpp
    # leanstore/concurrency/GroupCommitter.cpp
    # leanstore/concurrency/HistoryStorage.cpp
    # leanstore/concurrency/Logging.cpp
    # leanstore/concurrency/Recovery.cpp
    # leanstore/concurrency/WorkerContext.cpp
    # leanstore/btree/BasicKV.cpp
    # leanstore/btree/ChainedTuple.cpp
    # leanstore/btree/TransactionKV.cpp
    # leanstore/btree/Tuple.cpp
    # leanstore/btree/core/BTreeGeneric.cpp
    # leanstore/btree/core/BTreeNode.cpp
    # leanstore/btree/core/BTreeWalPayload.cpp
    # leanstore/leanstore-c/leanstore-c.cpp
    # leanstore/leanstore-c/StoreOption.cpp
    # leanstore/telemetry/MetricsHttpExposer.cpp
    #minisql
    minisql/buffer/buffer_pool_manager.cpp
    minisql/buffer/lru_replacer.cpp
    minisql/catalog/catalog.cpp
    minisql/catalog/indexes.cpp
    minisql/catalog/table.cpp
    minisql/executor/execute_engine.cpp
    minisql/index/b_plus_tree.cpp
    minisql/index/b_plus_tree_index.cpp
    minisql/index/index_iterator.cpp
    minisql/page/b_plus_tree_internal_page.cpp
    minisql/page/b_plus_tree_leaf_page.cpp
    minisql/page/b_plus_tree_page.cpp
    minisql/page/bitmap_page.cpp
    minisql/page/index_roots_page.cpp
    minisql/page/table_page.cpp
    minisql/record/column.cpp
    minisql/record/row.cpp
    minisql/record/schema.cpp
    minisql/record/types.cpp
    minisql/storage/disk_manager.cpp
    minisql/storage/table_heap.cpp
    minisql/storage/table_iterator.cpp
    minisql/parser/parser.c
    minisql/parser/parser.h
    minisql/parser/syntax_tree.c
    minisql/parser/syntax_tree.h
    minisql/parser/syntax_tree_printer.cpp
    minisql/parser/syntax_tree_printer.h
    minisql/parser/minisql_lex.c
    minisql/parser/minisql_yacc.c
    )
SET(MAIN_SRC Main.cpp)
# 将非Main.cpp文件添加到LIB_SRC中
# 这种类似rust中的lib.rs
FOREACH (F ${ALL_SRC})
    IF (NOT ${F} STREQUAL ${MAIN_SRC})
        LIST(APPEND LIB_SRC ${F})
    ENDIF ()
    MESSAGE("Use " ${F})
ENDFOREACH (F)

# 编译main函数
add_executable(main ${MAIN_SRC})
target_link_libraries(main lib_static)

add_library(lib_static STATIC ${LIB_SRC})
target_link_libraries(lib_static pthread ssl crypto sqlite3 tbb)