// Copyright 2014 BitPay Inc.
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cassert>
#include <string>
#include "univalue.h"

#ifndef JSON_TEST_SRC
#error JSON_TEST_SRC must point to test source directory
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif

using namespace std;
string srcdir(JSON_TEST_SRC);
static bool test_failed = false;

#define d_assert(expr) { if (!(expr)) { test_failed = true; fprintf(stderr, "%s failed\n", filename.c_str()); } }

static std::string rtrim(std::string s)
{
    s.erase(s.find_last_not_of(" \n\r\t")+1);
    return s;
}

static void runtest(string filename, const string& jdata)
{
        string prefix = filename.substr(0, 4);

        bool wantPass = (prefix == "pass") || (prefix == "roun");
        bool wantFail = (prefix == "fail");
        bool wantRoundTrip = (prefix == "roun");
        assert(wantPass || wantFail);

        UniValue val;
        bool testResult = val.read(jdata);

        if (wantPass) {
            d_assert(testResult == true);
        } else {
            d_assert(testResult == false);
        }

        if (wantRoundTrip) {
            std::string odata = val.write(0, 0);
            assert(odata == rtrim(jdata));
        }
}

static void runtest_file(const char *filename_)
{
        string basename(filename_);
        string filename = srcdir + "/" + basename;
        FILE *f = fopen(filename.c_str(), "r");
        assert(f != NULL);

        string jdata;

        char buf[4096];
        while (!feof(f)) {
                int bread = fread(buf, 1, sizeof(buf), f);
                assert(!ferror(f));

                string s(buf, bread);
                jdata += s;
        }

        assert(!ferror(f));
        fclose(f);

        runtest(basename, jdata);
}

static const char *filenames[] = {
        "fail10.json",
        "fail11.json",
        "fail12.json",
        "fail13.json",
        "fail14.json",
        "fail15.json",
        "fail16.json",
        "fail17.json",
        //"fail18.json",             // investigate
        "fail19.json",
        "fail1.json",
        "fail20.json",
        "fail21.json",
        "fail22.json",
        "fail23.json",
        "fail24.json",
        "fail25.json",
        "fail26.json",
        "fail27.json",
        "fail28.json",
        "fail29.json",
        "fail2.json",
        "fail30.json",
        "fail31.json",
        "fail32.json",
        "fail33.json",
        "fail34.json",
        "fail35.json",
        "fail36.json",
        "fail37.json",
        "fail3.json",
        "fail4.json",                // extra comma
        "fail5.json",
        "fail6.json",
        "fail7.json",
        "fail8.json",
        "fail9.json",               // extra comma
        "pass1.json",
        "pass2.json",
        "pass3.json",
        "round1.json",              // round-trip test
};

int main (int argc, char *argv[])
{
    for (unsigned int fidx = 0; fidx < ARRAY_SIZE(filenames); fidx++) {
        runtest_file(filenames[fidx]);
    }

    return test_failed ? 1 : 0;
}

