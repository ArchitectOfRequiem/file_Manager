#pragma once
#include<string>
#include<vector>
#include<fstream>
#include<filesystem>
#include<string>
#include<stdlib.h>
#include<utility>

#include "Utillity_01.h"
class fileManagerBase
{
public:
	fileManagerBase() = default;
	fileManagerBase(const std::string& filePath, std::ios_base::openmode flags);
	~fileManagerBase();


	virtual std::vector<std::string> readFromFile();
	virtual void writeToFile(const std::string& inContent);
	virtual void appendToFile(const std::string& appContent);

	virtual bool openFile(const std::string& filePath, std::ios_base::openmode flags);

	bool createFile(const std::string& filePath);
	bool itsOpen() const;
	virtual void changeDestinationFile(const std::string& filePath, std::ios_base::openmode flags);

	std::string GetProjectPath() const;
	std::string GetFilesPath() const;
protected:
	std::fstream* file;
	std::string filePath;
};

class fileReader : public fileManagerBase
{
public:
	fileReader() = default;
	fileReader(const std::string& filePath);
	fileReader(class fileWriter& fileReader);
	~fileReader();
	std::vector<std::string> readFromFile() override;
	bool openFile(const std::string& filePath, std::ios_base::openmode flags = std::ios::in) override;
	
};

class fileWriter : public fileManagerBase
{
public:
	fileWriter();
	fileWriter(const std::string& filePath, bool shouldBeAppend = true);
	~fileWriter();
	void writeToFile(const std::string& inContent) override;
	bool openFile(const std::string& filePath, std::ios_base::openmode flags = std::ios::out) override;
	void ClearFile(const std::string& filePath);

	inline void SetAppendFile(bool val) { this->Append = val; }
	inline std::fstream& GetStreamInst() const { return *this->file; }
	inline std::string GetCurrentFilePath() const { return this->filePath; }

private:
	bool Append = false;
};