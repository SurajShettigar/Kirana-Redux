// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_CORE_FILE_MANAGER_HPP
#define KIRANA_CORE_FILE_MANAGER_HPP

#include <filesystem>
#include <fstream>

namespace kirana::core
{
typedef std::filesystem::path Filepath;
typedef std::ifstream FileStream;
typedef std::ofstream FileStreamOut;

static bool fileExists(const Filepath &filepath)
{
    return std::filesystem::exists(filepath);
}

static std::string getFilename(const Filepath &filepath)
{
    return filepath.filename().string();
}

static std::string getExtension(const Filepath &filepath)
{
    const std::string ext = filepath.extension().string();
    return ext.empty() ? ext : ext.substr(1, ext.size());
}

static Filepath getAbsoluteFilepath(const Filepath &filepath)
{
    return std::filesystem::absolute(filepath);
}

static size_t getFileSize(const Filepath &filepath)
{
    return std::filesystem::file_size(filepath);
}

static FileStream openFileToRead(const Filepath &filepath, const bool is_binary = false, const bool seek_eof = false)
{
    const std::ifstream::openmode mode =
        (is_binary ? std::ios::binary : 0) | (seek_eof ? std::ios::ate : 0);
    return std::ifstream(filepath.string(), mode);
}

static FileStreamOut openFileToWrite(const Filepath &filepath, const bool is_binary = false)
{
    const std::ofstream::openmode mode = std::ios::out | std::ios::trunc | (is_binary ? std::ios::binary : 0);
    return std::ofstream(filepath.string(), mode);
}

static void closeFile(FileStream &stream)
{
    stream.close();
}

static void closeFile(FileStreamOut &stream)
{
    stream.close();
}

static void readFileChunk(FileStream &stream, const size_t offset, const size_t chunk_size, char *out_data)
{
    stream.seekg(static_cast<std::ifstream::off_type>(offset), std::ios::beg);
    stream.read(out_data, static_cast<std::streamsize>(chunk_size));
}

static bool readFileChunk(const Filepath &filepath, const bool is_binary, const size_t offset, const size_t chunk_size,
                          char *out_data)
{
    if (!fileExists(filepath) || out_data == nullptr)
        return false;

    std::ifstream stream = openFileToRead(filepath, is_binary, true);
    const size_t size = stream.tellg();
    if (offset + chunk_size > size)
        return false;

    readFileChunk(stream, offset, chunk_size, out_data);
    closeFile(stream);
    return true;
}

static size_t readFile(const Filepath &filepath, const bool is_binary, char *out_data)
{
    if (!fileExists(filepath))
        return 0;

    std::ifstream stream = openFileToRead(filepath, is_binary, true);
    const size_t size = stream.tellg();
    if (out_data == nullptr)
        return size;

    readFileChunk(stream, 0, size, out_data);
    closeFile(stream);
    return size;
}

static bool writeFile(const Filepath &filepath, const bool is_binary, const size_t size, const char *data)
{
    FileStreamOut stream = openFileToWrite(filepath, is_binary);
    stream.write(data, size);
    closeFile(stream);
    return true;
}
}

#endif  // KIRANA_CORE_FILE_MANAGER_HPP