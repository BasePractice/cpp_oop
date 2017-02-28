#include <fstream>
#include <vector>
#include <strstream>
#include <gtest/gtest.h>
#include "common_tests.h"

const std::string
file_content(const char *const filename, bool *ok) {
    std::ifstream stream;
    std::stringstream buffer;

    (*ok) = false;
    stream.open(std::string(filename), std::ios_base::in);
    if (stream.is_open()) {
        (*ok) = true;
        buffer << stream.rdbuf();
        return buffer.str();
    }
    return "";
}

const std::vector<std::string>
file_content_lines(const char *const filename, bool *ok) {
    std::vector<std::string> result;
    auto content = file_content(filename, ok);
    if ((*ok)) {
        std::istringstream stream(content);
        std::copy(std::istream_iterator<std::string>(stream),
                  std::istream_iterator<std::string>(),
                  std::back_inserter<std::vector<std::string> >(result));
    }
    return result;
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
#if defined(MEMORY_LEAK_DETECT)
    _CrtMemState _checkpoint_start;
    _CrtMemState _checkpoint_end;
    _CrtMemState _checkpoint_diff;

    fprintf(stderr, "Memory leak detection enabled\n");
     _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
     _CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_FILE );
     _CrtSetReportFile( _CRT_WARN, _CRTDBG_FILE_STDERR );
    _CrtMemCheckpoint(&_checkpoint_start);

#endif
    int ret = RUN_ALL_TESTS();
#if defined(MEMORY_LEAK_DETECT)
    _CrtMemCheckpoint(&_checkpoint_end);
    if (_CrtMemDifference(&_checkpoint_diff, &_checkpoint_start, &_checkpoint_end))
        _CrtMemDumpStatistics( &_checkpoint_diff );
#endif
    return ret;
}