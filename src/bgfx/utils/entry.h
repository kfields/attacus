#pragma once

#include <bx/bx.h>
#include <bx/filepath.h>
#include <bx/file.h>
#include <bx/string.h>

namespace entry {

void init();

bx::FileReaderI* getFileReader();
bx::FileWriterI* getFileWriter();
bx::AllocatorI*  getAllocator();

}