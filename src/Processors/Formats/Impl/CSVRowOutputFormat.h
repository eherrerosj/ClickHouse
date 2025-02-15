#pragma once

#include <Core/Block.h>
#include <Processors/Formats/IRowOutputFormat.h>
#include <Formats/FormatSettings.h>


namespace DB
{

class WriteBuffer;


/** The stream for outputting data in csv format.
  * Does not conform with https://tools.ietf.org/html/rfc4180 because it uses LF, not CR LF.
  */
class CSVRowOutputFormat final : public IRowOutputFormat
{
public:
    /** with_names - output in the first line a header with column names
      * with_types - output in the next line header with the names of the types
      */
    CSVRowOutputFormat(WriteBuffer & out_, const Block & header_, bool with_names_, bool with_types, const FormatSettings & format_settings_);

    String getName() const override { return "CSVRowOutputFormat"; }

    /// https://www.iana.org/assignments/media-types/text/csv
    String getContentType() const override
    {
        return String("text/csv; charset=UTF-8; header=") + (with_names ? "present" : "absent");
    }

private:
    void writeField(const IColumn & column, const ISerialization & serialization, size_t row_num) override;
    void writeFieldDelimiter() override;
    void writeRowBetweenDelimiter() override;

    bool supportTotals() const override { return true; }
    bool supportExtremes() const override { return true; }

    void writeBeforeTotals() override;
    void writeAfterTotals() override;
    void writeBeforeExtremes() override;
    void writeAfterExtremes() override;

    void writePrefix() override;
    void writeSuffix() override;
    void writeLine(const std::vector<String> & values);

    bool with_names;
    bool with_types;
    const FormatSettings format_settings;
    DataTypes data_types;
};

}
