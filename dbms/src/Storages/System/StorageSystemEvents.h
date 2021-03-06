#pragma once

#include <ext/shared_ptr_helper.hpp>
#include <Storages/IStorage.h>


namespace DB
{

class Context;


/** Implements `events` system table, which allows you to obtain information for profiling.
  */
class StorageSystemEvents : private ext::shared_ptr_helper<StorageSystemEvents>, public IStorage
{
friend class ext::shared_ptr_helper<StorageSystemEvents>;

public:
    static StoragePtr create(const std::string & name_);

    std::string getName() const override { return "SystemEvents"; }
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

    StorageSystemEvents(const std::string & name_);
};

}
