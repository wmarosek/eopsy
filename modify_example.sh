#-------------------------------------------------------------------------------------
#   Create testing data for basic tests
#-------------------------------------------------------------------------------------
mkdir test_dir
cd test_dir
touch c_program.c
touch upper_testfile
touch LOWERCASE_TESTFILE.b

mkdir test_subdir
cd test_subdir
touch test_file2.cpp
cd ..
cd ..

#-------------------------------------------------------------------------------------
#   Test 1.1 - uppercase given file [upper_testfile]
#-------------------------------------------------------------------------------------
echo "
=============================================================================
Test 1.1  -   uppercase given file [upper_testfile]

            using: ./modify.sh -u test_dir/upper_testfile
============================================================================="
echo "Current (before test execution) dictionary tree"
ls test_dir
./modify.sh -u test_dir/upper_testfile

echo "=============================================================================
Current (after test execution) dictionary tree"
ls test_dir


#-------------------------------------------------------------------------------------
#   Test 1.2 - lowercase given file [LOWERCASE_TESTFILE.b]
#-------------------------------------------------------------------------------------
echo "
=============================================================================
Test 1.2  -   lowercase given file [LOWERCASE_TESTFILE.b]

            using: ./modify.sh -u test_dir/LOWERCASE_TESTFILE.b
============================================================================="
echo "Current (before test execution) dictionary tree"
ls test_dir
./modify.sh -l test_dir/LOWERCASE_TESTFILE.b

echo "=============================================================================
Current (after test execution) dictionary tree"
ls test_dir


#-------------------------------------------------------------------------------------
#   Test 1.3 - uppercase few given files [lowercase_testfile.b , c_program.c]
#-------------------------------------------------------------------------------------
echo "
=============================================================================
Test 1.3  -   uppercase few given files [lowercase_testfile.b , c_program.c]

            using: ./modify.sh -u test_dir/c_program.c test_dir/lowercase_testfile.b 
============================================================================="
echo "Current (before test execution) dictionary tree"
ls test_dir
./modify.sh -u test_dir/c_program.c test_dir/lowercase_testfile.b 

echo "=============================================================================
Current (after test execution) dictionary tree"
ls test_dir


#-------------------------------------------------------------------------------------
#   Test 1.4 - lowercase few given files [lowercase_testfile.b , c_program.c]
#-------------------------------------------------------------------------------------
echo "
=============================================================================
Test 1.4  -   lowercase few given files [LOWERCASE_TESTFILE.b , C_PROGRAM.c]

            using: ./modify.sh -l test_dir/C_PROGRAM.c test_dir/LOWERCASE_TESTFILE.b 
============================================================================="
echo "Current (before test execution) dictionary tree"
ls test_dir
./modify.sh -l test_dir/C_PROGRAM.c test_dir/LOWERCASE_TESTFILE.b

echo "=============================================================================
Current (after test execution) dictionary tree"
ls test_dir


#-------------------------------------------------------------------------------------
#   Test 1.5 - sed pattern given file [c_program.c]
#-------------------------------------------------------------------------------------
echo "
=============================================================================
Test 1.5  -   sed pattern given file [test_file2.cpp]

            using: ./modify.sh 's/test/test_success/g' test_dir/test_subdir/test_file2.cpp 
============================================================================="
echo "Current (before test execution) dictionary tree"
ls -R test_dir
./modify.sh 's/test/test_success/g' test_dir/test_subdir/test_file2.cpp 

echo "=============================================================================
Current (after test execution) dictionary tree"
ls -R test_dir


#-------------------------------------------------------------------------------------
#   Create testing data for recursive tests
#-------------------------------------------------------------------------------------

mkdir -p test_rec_dir/test_rec_subdir/test_rec_sub_subdir
mkdir -p test_rec_dir/test_rec_subdir2

touch test_rec_dir/test_rec_file
touch test_rec_dir/test_rec_file2.txt

touch test_rec_dir/test_rec_subdir/test_rec_sub_file.h
touch test_rec_dir/test_rec_subdir/test_rec_sub_file2.cpp
touch test_rec_dir/test_rec_subdir/test_rec_sub_subdir/test_rec_sub_sub_file.h


touch test_rec_dir/test_rec_subdir2/test_rec_sub2_file.eopsy
touch test_rec_dir/test_rec_subdir2/test_rec_sub2_file2.cpp


#-------------------------------------------------------------------------------------
#   Test 2.1 - recursive uppercase of given dictonary [test_rec_dir]
#-------------------------------------------------------------------------------------
echo "
=============================================================================
Test 2.1  -   recursive uppercase of given dictonary [test_rec_dir]

            using: ./modify.sh -r -u test_rec_dir 
============================================================================="
echo "Current (before test execution) dictionary tree"
ls -R test_rec_dir
./modify.sh -r -u test_rec_dir 

echo "=============================================================================
Current (after test execution) dictionary tree"
ls -R test_rec_dir


#-------------------------------------------------------------------------------------
#   Test 2.2 - recursive lowercase of given dictonary [test_rec_dir]
#-------------------------------------------------------------------------------------
echo "
=============================================================================
Test 2.2  -   recursive lowercase of given dictonary [test_rec_dir]

            using: ./modify.sh -r -l test_rec_dir 
============================================================================="
echo "Current (before test execution) dictionary tree"
ls -R test_rec_dir
./modify.sh -r -l test_rec_dir 

echo "=============================================================================
Current (after test execution) dictionary tree"
ls -R test_rec_dir

#-------------------------------------------------------------------------------------
#   Test 2.3 - recursive sed pattern given dictonary [test_rec_dir]
#-------------------------------------------------------------------------------------
echo "
=============================================================================
Test 2.3  -   recursive sed pattern given dictonary [test_rec_dir]

            using: ./modify.sh -r 's/test/test_success/g' test_rec_dir
============================================================================="
echo "Current (before test execution) dictionary tree"
ls -R test_rec_dir
./modify.sh -r 's/test/test_success/g' test_rec_dir

echo "=============================================================================
Current (after test execution) dictionary tree"
ls -R test_rec_dir



# -------------------------------------------------------------------------------------
#   Test 3.1 - improper usage - nonexisting file [improper_filename]
# -------------------------------------------------------------------------------------
echo "
=============================================================================
Test 3.1  -   improper usage - nonexisting file [improper_filename]

            using: ./modify.sh -u test_dir/improper_filename
============================================================================="
echo "Current (before test execution) dictionary tree"
ls test_dir
echo "============================================================================="

./modify.sh -u test_dir/improper_filename

echo "=============================================================================
Current (after test execution) dictionary tree"
ls test_dir

# -------------------------------------------------------------------------------------
#   Test 3.2 - improper usage - nonexisting directory [improper_filename]
# -------------------------------------------------------------------------------------
echo "
=============================================================================
Test 3.2  -   improper usage - nonexisting directory [improper_filename]

            using: ./modify.sh -u improper_dir/test_file2.cpp
============================================================================="
echo "Current (before test execution) dictionary tree"
ls test_dir
echo "============================================================================="

./modify.sh -u improper_dir/test_file2.cpp

echo "=============================================================================
Current (after test execution) dictionary tree"
ls test_dir

# -------------------------------------------------------------------------------------
#   Test 3.3 - lowercase and uppercase flag [upper_testfile]
# -------------------------------------------------------------------------------------
echo "
=============================================================================
Test 3.3  -   lowercase and uppercase flag [UPPER_TESTFILE]

            using: ./modify.sh -u -l test_dir/UPPER_TESTFILE
============================================================================="
echo "Current (before test execution) dictionary tree"
ls -R test_dir
echo "============================================================================="
./modify.sh -u -l test_dir/UPPER_TESTFILE
echo "=============================================================================
Current (after test execution) dictionary tree"
ls -R test_dir
