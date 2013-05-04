#pragma once

#include <Poco/SharedPtr.h>

#include <DB/Interpreters/Expression.h>
#include <DB/DataStreams/IProfilingBlockInputStream.h>
#include <DB/Columns/ColumnConst.h>



namespace DB
{


/** Добавляет в блок недостающие столбцы со значениями по-умолчанию.
  * Эти столбцы - материалированные (не константы).
  */
class AddingDefaultBlockInputStream : public IProfilingBlockInputStream
{
public:
	AddingDefaultBlockInputStream(
		BlockInputStreamPtr input_,
		NamesAndTypesListPtr required_columns_)
		: required_columns(required_columns_)
	{
		children.push_back(input_);
		input = &*children.back();
	}

	String getName() const { return "AddingDefaultBlockInputStream"; }

	String getID() const
	{
		std::stringstream res;
		res << "AddingDefault(" << input->getID();

		for (NamesAndTypesList::const_iterator it = required_columns->begin(); it != required_columns->end(); ++it)
			res << ", " << it->first << ", " << it->second->getName();

		res << ")";
		return res.str();
	}

protected:
	Block readImpl()
	{
		Block res = input->read();
		if (!res)
			return res;

		for (NamesAndTypesList::const_iterator it = required_columns->begin(); it != required_columns->end(); ++it)
		{
			if (!res.has(it->first))
			{
				ColumnWithNameAndType col;
				col.name = it->first;
				col.type = it->second;
				col.column = dynamic_cast<IColumnConst &>(*it->second->createConstColumn(
					res.rows(), it->second->getDefault())).convertToFullColumn();
				res.insert(col);
			}
		}

		return res;
	}

private:
	IBlockInputStream * input;
	NamesAndTypesListPtr required_columns;
};

}
