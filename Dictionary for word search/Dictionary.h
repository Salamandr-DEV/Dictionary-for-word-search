#pragma once

#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <filesystem>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/filereadstream.h>

class Dictionary
{
public:
	Dictionary();
	~Dictionary();

	void addWord(const std::string &word, const std::string &definition) 
    {
		dictionary[word] = definition;
	}

	void findWord(const std::string &word) 
    {
		auto it = dictionary.find(word);
		if (it != dictionary.end())
		{
			std::cout << word << ": " << it->second << std::endl;
		}
		else
		{
			std::cout << word << " not found." << std::endl;
		}
	}

    void saveToJsonFile() 
    {
        // Создание JSON-документа
        rapidjson::Document doc;
        doc.SetObject();

        rapidjson::Value Dictionary_JSON;
        Dictionary_JSON.SetString("Dictionary", doc.GetAllocator());

        rapidjson::Value Dictionary_Element;
        Dictionary_Element.SetObject();

        // Заполнение JSON-документа данными из std::map
        for (const auto &pair : dictionary) 
        {
            rapidjson::Value word;
            rapidjson::Value definition;

            word.SetString(pair.first.data(), pair.first.size(), doc.GetAllocator());
            definition.SetString(pair.second.data(), pair.second.size(), doc.GetAllocator());
            Dictionary_Element.AddMember(word, definition, doc.GetAllocator());
        }

        doc.AddMember(Dictionary_JSON, Dictionary_Element, doc.GetAllocator());

        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        doc.Accept(writer);
        const size_t json_size = buffer.GetSize();

        std::fstream out;
        out.open("Dictionary.json", std::ios::binary | std::ios::out);
        out.seekp(0, std::ios_base::end);
        out.write(reinterpret_cast<const char*>(buffer.GetString()), sizeof(char) * json_size);
        out.close();
    }

    //Функция для чтения данных из JSON-файла в std::map
    void loadFromJsonFile() 
    {
        FILE* fp = _wfopen(L"Dictionary.json", L"rb");
        if (fp == NULL) return;
        char readBuffer[65536] = { 0 };
        rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
        rapidjson::Document doc;
        doc.ParseStream(is);
        if (doc.HasParseError())
        {
            fclose(fp);
            throw std::runtime_error("Некорректный JSON!");
        }
        fclose(fp);

        // Извлечение данных из JSON-документа в std::map

        const auto it = doc.FindMember("Dictionary");
        rapidjson::Value Dictionary_JSON = it->value.GetObject();

        for (auto itr = Dictionary_JSON.MemberBegin(); itr != Dictionary_JSON.MemberEnd(); ++itr) 
        {
            dictionary[itr->name.GetString()] = itr->value.GetString();
        }
    }

	std::map<std::string, std::string> &getDictionary()
	{
		return dictionary;
	}

private:
	std::map<std::string, std::string> dictionary;
};

Dictionary::Dictionary()
{
}

Dictionary::~Dictionary()
{
}