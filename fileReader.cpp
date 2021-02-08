#include "fileReader.h"


fileManagerBase::fileManagerBase(const std::string& filePath, std::ios_base::openmode flags)
{
	this->filePath = filePath;
	this->file = new std::fstream(filePath, flags);

	LOG("Constructor of FileManager base called");

}

fileManagerBase::~fileManagerBase()
{
	if (this->itsOpen())
	{
		LOG("Base destructor called");
		this->file->flush();
		this->file->close();
	}
}

std::vector<std::string> fileManagerBase::readFromFile()
{
	return std::vector<std::string>();
}

void fileManagerBase::writeToFile(const std::string& inContent)
{

}

void fileManagerBase::appendToFile(const std::string& appContent)
{
}

bool fileManagerBase::openFile(const std::string& filePath, std::ios_base::openmode flags)
{
	if (filePath.length() == 0) return false;

	if (!this->file)
	{
		this->file = new std::fstream(filePath, flags);
	}
	this->filePath = filePath;

	if (itsOpen())
	{
		this->file->flush();
		this->file->close();
	}

	this->file->open(filePath, flags);
	return this->file->is_open();
}

bool fileManagerBase::createFile(const std::string& filePath)
{
	std::ofstream create(filePath);
	if (create)
	{
		create.close();
		return true;
	}
	return false;
}

bool fileManagerBase::itsOpen() const
{
	if (!file) return false;
	return file->is_open();
}

void fileManagerBase::changeDestinationFile(const std::string& filePath, std::ios_base::openmode flags)
{
}

std::string fileManagerBase::GetProjectPath() const
{
	return std::filesystem::current_path().string();
}

std::string fileManagerBase::GetFilesPath() const
{
	std::string ProjectPath = GetProjectPath();
	std::string FilesExtPath = "\\Files";
	std::string FilesPath = ProjectPath.append(FilesExtPath);

	return FilesPath;
}

//---------------FILE READER
fileReader::fileReader(const std::string& filePath)
{
	LOG("fileReader constructor called");
	this->filePath = filePath;
	this->file = new std::fstream(filePath, std::ios::in);
}

fileReader::fileReader(fileWriter& fileWriter)
{
	this->file = &fileWriter.GetStreamInst();
	this->filePath = fileWriter.GetCurrentFilePath();
}

fileReader::~fileReader()
{
	LOG("fileReader destructor called");
	fileManagerBase::~fileManagerBase();
}

std::vector<std::string> fileReader::readFromFile()
{
	if (this->file)
	{
		if (!this->itsOpen() && this->filePath.length() != 0)
		{
			this->openFile(this->filePath);
		}
	}
	else return std::vector<std::string>();


	std::string outLine;
	std::vector<std::string> outFileContent;
	while (std::getline(*this->file, outLine))
	{
		outFileContent.push_back(outLine);
	}
	this->file->close();
	return outFileContent;
}

bool fileReader::openFile(const std::string& filePath, std::ios_base::openmode flags)
{
	return fileManagerBase::openFile(filePath, std::ios::in);
}


//--------------FILE READER
fileWriter::fileWriter()
{
	//By default file will be append
	SetAppendFile(true);
}
fileWriter::fileWriter(const std::string& filePath, bool shouldBeAppend)
{
	this->filePath = filePath;
	this->Append = shouldBeAppend;
	this->file = new std::fstream(filePath, Append ? std::ios::app : std::ios::out);
}

fileWriter::~fileWriter()
{
	fileManagerBase::~fileManagerBase();
}

void fileWriter::writeToFile(const std::string& inContent)
{
	if (inContent.length() == 0) return;
	if (!Append) this->ClearFile(this->filePath);
	if (this->itsOpen())
	{
		*file << inContent;
	}
	else
	{
		if (this->filePath.length() != 0)
		{
			this->openFile(filePath);
			*file << inContent;
		}
	}
	file->close();
}

bool fileWriter::openFile(const std::string& filePath, std::ios_base::openmode flags)
{
	this->filePath = filePath;
	return fileManagerBase::openFile(filePath, Append ? std::ios::app : std::ios::out);
}

void fileWriter::ClearFile(const std::string& filePath)
{
	if (this->file)
	{
		if (this->itsOpen())
		{
			this->file->flush();
			this->file->close();
		}
	}
	else return;

	std::ofstream* clearFile = new std::ofstream(filePath, std::ios::out);
	clearFile->close();

	fileManagerBase::openFile(filePath, Append ? std::ios::app : std::ios::out);
}
