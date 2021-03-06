#pragma once

#include <ext/shared_ptr_helper.hpp>
#include <Storages/IStorage.h>


namespace DB
{

class Context;


/** Implements the `replication_queue` system table, which allows you to view the replication queues for the replicated tables.
  */
class StorageSystemReplicationQueue : private ext::shared_ptr_helper<StorageSystemReplicationQueue>, public IStorage
{
friend class ext::shared_ptr_helper<StorageSystemReplicationQueue>;

public:
    static StoragePtr create(const std::string & name_);

    std::string getName() const override { return "SystemReplicationQueue"; }
    std::string getTableName() const override { return name; }

    const NamesAndTypesList & getColumnsListImpl() const override { return columns; }

    BlockInputStreams read(
        const Names & column_names,
        const ASTPtr & query,
        const Context & context,
        QueryProcessingStage::Enum & processed_stage,
        size_t max_block_size,
        unsigned num_streams) override;

private:
    const std::string name;
    NamesAndTypesList columns;

    StorageSystemReplicationQueue(const std::string & name_);
};

}
