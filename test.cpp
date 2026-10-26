
// CS 405 Module Four Milestone
// Brandon Canady
// Modernized Unit Testing with Google Test (C++17)
//
// Purpose:
// This program uses Google Test to verify the behavior of a C++ vector.
// The tests check collection initialization, insertion, resizing,
// memory capacity, element removal, and exception handling.
//
// The program includes 11 required tests and 2 custom tests.
// The custom tests verify correct insertion order and ensure that
// invalid index access does not modify the collection.

// ---------------------------------------------------------
// HEADER FILES
// ---------------------------------------------------------

// Use the precompiled header configured in Visual Studio.
// This header typically includes the Google Test dependencies.
#include "pch.h"

// Uncomment this line if the project does not use precompiled headers.
// #include "gtest/gtest.h"

#include <vector>       // Provides std::vector for storing integer values
#include <memory>       // Provides smart pointers and std::make_unique
#include <random>       // Provides modern C++ random number generation
#include <stdexcept>    // Provides standard exceptions such as std::out_of_range

// ---------------------------------------------------------
// GLOBAL TEST ENVIRONMENT
// ---------------------------------------------------------

// This class provides setup and cleanup methods that apply to the
// overall Google Test environment.
//
// Global setup runs before the test suite, while global cleanup
// runs after testing has finished.
//
// Because this version uses the modern <random> library instead
// of rand(), global random number initialization is unnecessary.

class Environment : public ::testing::Environment
{
public:

    // Executes before the tests begin.
    // No global initialization is required for this program.
    void SetUp() override
    {
    }

    // Executes after testing has completed.
    // No additional global cleanup is required.
    void TearDown() override
    {
    }
};

// ---------------------------------------------------------
// TEST FIXTURE
// ---------------------------------------------------------

// CollectionTest is a test fixture that provides shared setup,
// cleanup, and helper functions for the individual unit tests.
//
// Each TEST_F creates a new instance of this fixture, allowing
// every test to use its own collection.
//
// This prevents changes made during one test from affecting
// the results of another test.

class CollectionTest : public ::testing::Test
{
protected:

    // Smart pointer used to manage the vector's memory.
    //
    // unique_ptr automatically releases the allocated vector
    // when the pointer goes out of scope.
    //
    // This avoids manually calling delete and reduces the
    // possibility of memory management errors.
    std::unique_ptr<std::vector<int>> collection;

    // Mersenne Twister random number generator.
    // This replaces the older rand() function.
    std::mt19937 rng;

    // Defines the range of random integer values.
    // The distribution will produce values from 0 through 99.
    std::uniform_int_distribution<int> dist;

    // -----------------------------------------------------
    // SETUP
    // -----------------------------------------------------

    // SetUp() executes automatically before each TEST_F.
    //
    // It creates a new empty vector and prepares the random
    // number generator for tests that require sample values.

    void SetUp() override
    {
        // Create the vector using std::make_unique.
        //
        // This uses modern C++ memory management instead of
        // directly allocating memory with the new keyword.
        collection = std::make_unique<std::vector<int>>();

        // Initialize the random number generator.
        //
        // std::random_device supplies a seed so that the
        // generated values can vary between test runs.
        //
        // The tests focus on vector behavior rather than
        // depending on specific randomly generated values.
        rng = std::mt19937(std::random_device{}());

        // Configure random values between 0 and 99.
        //
        // Both endpoints are included in the distribution.
        dist = std::uniform_int_distribution<int>(0, 99);
    }

    // -----------------------------------------------------
    // TEARDOWN
    // -----------------------------------------------------

    // TearDown() executes automatically after each test.
    //
    // Clearing the vector removes its stored elements.
    // The unique_ptr automatically releases the vector when
    // the test fixture is destroyed.

    void TearDown() override
    {
        // Remove all elements from the collection.
        // This is optional because unique_ptr manages cleanup.
        collection->clear();
    }

    // -----------------------------------------------------
    // HELPER FUNCTION: ADD RANDOM ENTRIES
    // -----------------------------------------------------

    // Adds a requested number of random integers to the vector.
    //
    // This helper avoids repeating insertion code throughout
    // the test suite.
    //
    // Parameter:
    // count - number of integer values to add.
    //
    // A positive count is required because adding zero or
    // a negative number would not satisfy the intended operation.

    void add_entries(int count)
    {
        // Verify that the requested number of entries is positive.
        //
        // ASSERT_GT terminates this helper function if count
        // is not greater than zero.
        //
        // Unlike a standard assert(), Google Test records the
        // failure in the test results.
        ASSERT_GT(count, 0) << "Count must be positive.";

        // Repeat the insertion operation count times.
        for (int i = 0; i < count; ++i)
        {
            // Generate a random integer and append it to the vector.
            //
            // emplace_back constructs the element at the end
            // of the collection.
            //
            // For a basic integer, emplace_back and push_back
            // provide equivalent insertion behavior.
            collection->emplace_back(dist(rng));
        }
    }
};

// ---------------------------------------------------------
// EXISTING TEST 1: VERIFY SMART POINTER
// ---------------------------------------------------------

// Purpose:
// Confirm that the test fixture successfully creates the vector.
//
// A null pointer would make accessing the collection unsafe.
// The assertions verify that memory allocation succeeded
// before other tests attempt to use the collection.

TEST_F(CollectionTest, CollectionSmartPointerIsNotNull)
{
    // Confirm that the smart pointer owns a valid object.
    ASSERT_TRUE(collection);

    // Verify that the underlying raw pointer is not null.
    ASSERT_NE(collection.get(), nullptr);
}

// ---------------------------------------------------------
// EXISTING TEST 2: VERIFY EMPTY COLLECTION
// ---------------------------------------------------------

// Purpose:
// Confirm that a newly created vector contains no elements.
//
// A default-constructed vector should report empty() as true
// and have a size of zero.

TEST_F(CollectionTest, IsEmptyOnCreate)
{
    // Confirm that the vector starts empty.
    ASSERT_TRUE(collection->empty());

    // Verify that its initial element count is zero.
    ASSERT_EQ(collection->size(), 0);
}

// ---------------------------------------------------------
// REQUIRED TEST 1: ADD ONE VALUE
// ---------------------------------------------------------

// Purpose:
// Verify that one value can be added to an empty collection.
//
// Expected behavior:
// The collection starts empty and contains exactly one
// element after the insertion operation.

TEST_F(CollectionTest, CanAddToEmptyVector)
{
    // Verify the initial state before modifying the vector.
    ASSERT_TRUE(collection->empty());
    ASSERT_EQ(collection->size(), 0);

    // Insert one randomly generated integer.
    add_entries(1);

    // The vector should no longer be empty.
    EXPECT_FALSE(collection->empty());

    // Verify that the element count increased from 0 to 1.
    EXPECT_EQ(collection->size(), 1);
}

// ---------------------------------------------------------
// REQUIRED TEST 2: ADD FIVE VALUES
// ---------------------------------------------------------

// Purpose:
// Verify that multiple values can be inserted successfully.
//
// Expected behavior:
// After inserting five values, the vector should contain
// exactly five elements and should not be empty.

TEST_F(CollectionTest, CanAddFiveValuesToVector)
{
    // Confirm that the vector starts empty.
    ASSERT_TRUE(collection->empty());

    // Add five randomly generated integers.
    add_entries(5);

    // Verify that the collection now contains data.
    EXPECT_FALSE(collection->empty());

    // Confirm that all five values were inserted.
    EXPECT_EQ(collection->size(), 5);
}

// ---------------------------------------------------------
// REQUIRED TEST 3: VERIFY MAXIMUM SIZE
// ---------------------------------------------------------

// Purpose:
// Confirm that max_size() is greater than or equal to the
// number of elements currently stored in the vector.
//
// max_size() represents the maximum number of elements
// the vector could theoretically contain.
//
// This value is influenced by the implementation and
// allocator limits. It does not represent currently
// allocated storage.
//
// Test cases:
// 0 elements, 1 element, 5 elements, and 10 elements.

TEST_F(CollectionTest, MaxSizeIsGreaterThanOrEqualToSize)
{
    // Check the empty vector.
    // Its maximum supported size must be at least zero.
    EXPECT_GE(collection->max_size(), collection->size());

    // Add one element and check the size relationship.
    add_entries(1);

    EXPECT_GE(collection->max_size(), collection->size());

    // Add four more elements, bringing the total to five.
    add_entries(4);

    // Confirm that the expected number of elements exists.
    EXPECT_EQ(collection->size(), 5);

    // Maximum supported size must remain at least five.
    EXPECT_GE(collection->max_size(), collection->size());

    // Add another five elements, bringing the total to ten.
    add_entries(5);

    // Verify the final element count.
    EXPECT_EQ(collection->size(), 10);

    // Confirm the vector has not exceeded its maximum size.
    EXPECT_GE(collection->max_size(), collection->size());
}

// ---------------------------------------------------------
// REQUIRED TEST 4: VERIFY VECTOR CAPACITY
// ---------------------------------------------------------

// Purpose:
// Verify that the vector's allocated capacity is always
// greater than or equal to its current element count.
//
// size() reports the number of elements stored.
// capacity() reports how many elements can fit in the
// currently allocated storage before reallocation is needed.
//
// A vector may allocate extra space to support future
// insertions without reallocating memory every time.
//
// Test cases:
// 0 elements, 1 element, 5 elements, and 10 elements.

TEST_F(CollectionTest, CapacityIsGreaterThanOrEqualToSize)
{
    // Check the initial empty collection.
    EXPECT_GE(collection->capacity(), collection->size());

    // Insert one element.
    add_entries(1);

    // Verify that allocated capacity supports the element.
    EXPECT_GE(collection->capacity(), collection->size());

    // Increase the total element count to five.
    add_entries(4);

    // Confirm the expected collection size.
    EXPECT_EQ(collection->size(), 5);

    // Verify that capacity can hold all five elements.
    EXPECT_GE(collection->capacity(), collection->size());

    // Increase the total element count to ten.
    add_entries(5);

    // Confirm the final element count.
    EXPECT_EQ(collection->size(), 10);

    // Verify that allocated capacity is sufficient.
    EXPECT_GE(collection->capacity(), collection->size());
}

// ---------------------------------------------------------
// REQUIRED TEST 5: RESIZE INCREASES COLLECTION
// ---------------------------------------------------------

// Purpose:
// Verify that resize() can increase the number of elements
// stored in a vector.
//
// When a vector of integers is resized to a larger size,
// the additional elements are value-initialized to zero.
//
// Expected behavior:
// A collection containing five elements increases to ten,
// and the five new elements contain zero.

TEST_F(CollectionTest, ResizeIncreasesCollection)
{
    // Insert five elements into the vector.
    add_entries(5);

    // Confirm the starting size before resizing.
    ASSERT_EQ(collection->size(), 5);

    // Increase the logical size from five to ten elements.
    collection->resize(10);

    // Verify that the requested size was applied.
    EXPECT_EQ(collection->size(), 10);

    // Confirm that the vector still contains elements.
    EXPECT_FALSE(collection->empty());

    // Examine only the newly added elements.
    //
    // Index positions 0 through 4 contain the original values.
    // Index positions 5 through 9 are the new elements.
    for (size_t i = 5; i < collection->size(); ++i)
    {
        // Newly created integer elements should equal zero.
        EXPECT_EQ(collection->at(i), 0);
    }
}

// ---------------------------------------------------------
// REQUIRED TEST 6: RESIZE DECREASES COLLECTION
// ---------------------------------------------------------

// Purpose:
// Verify that resize() can reduce the number of elements.
//
// When resizing to a smaller value, elements beyond the
// requested size are removed from the collection.
//
// Expected behavior:
// A collection containing ten elements decreases to five.

TEST_F(CollectionTest, ResizeDecreasesCollection)
{
    // Populate the collection with ten values.
    add_entries(10);

    // Verify the original size.
    ASSERT_EQ(collection->size(), 10);

    // Reduce the collection to five elements.
    collection->resize(5);

    // Verify the new element count.
    EXPECT_EQ(collection->size(), 5);

    // Five elements remain, so the collection is not empty.
    EXPECT_FALSE(collection->empty());
}

// ---------------------------------------------------------
// REQUIRED TEST 7: RESIZE COLLECTION TO ZERO
// ---------------------------------------------------------

// Purpose:
// Verify that resizing a vector to zero removes all
// elements from the collection.
//
// This test confirms that the vector correctly updates
// its size and empty status.
//
// Note:
// resize(0) does not necessarily release allocated capacity.

TEST_F(CollectionTest, ResizeCollectionToZero)
{
    // Create a collection containing ten elements.
    add_entries(10);

    // Confirm that the collection contains ten values.
    ASSERT_EQ(collection->size(), 10);

    // Resize the collection to zero elements.
    collection->resize(0);

    // Verify that the collection is now empty.
    EXPECT_TRUE(collection->empty());

    // Confirm that no elements remain.
    EXPECT_EQ(collection->size(), 0);
}

// ---------------------------------------------------------
// REQUIRED TEST 8: CLEAR COLLECTION
// ---------------------------------------------------------

// Purpose:
// Verify that clear() removes every element from a vector.
//
// The clear() function destroys all stored elements
// and sets the logical size to zero.
//
// It does not guarantee that allocated capacity is released.

TEST_F(CollectionTest, ClearErasesCollection)
{
    // Insert five values into the collection.
    add_entries(5);

    // Confirm that the collection contains five elements.
    ASSERT_EQ(collection->size(), 5);

    // Remove every stored element.
    collection->clear();

    // Verify that the collection is empty.
    EXPECT_TRUE(collection->empty());

    // Confirm that its logical size is zero.
    EXPECT_EQ(collection->size(), 0);
}

// ---------------------------------------------------------
// REQUIRED TEST 9: ERASE ENTIRE COLLECTION
// ---------------------------------------------------------

// Purpose:
// Verify that erase(begin, end) removes the complete
// range of elements from the collection.
//
// begin() identifies the first element.
// end() identifies the position immediately after
// the final element.
//
// Since the erase range excludes the ending iterator,
// this removes every stored element.

TEST_F(CollectionTest, EraseBeginEndErasesCollection)
{
    // Insert five values into the collection.
    add_entries(5);

    // Verify that the vector contains five elements.
    ASSERT_EQ(collection->size(), 5);

    // Erase all elements between begin() and end().
    collection->erase(collection->begin(), collection->end());

    // Confirm that the vector no longer contains data.
    EXPECT_TRUE(collection->empty());

    // Verify that the element count is now zero.
    EXPECT_EQ(collection->size(), 0);
}

// ---------------------------------------------------------
// REQUIRED TEST 10: RESERVE ADDITIONAL CAPACITY
// ---------------------------------------------------------

// Purpose:
// Verify that reserve() increases the vector's allocated
// capacity without changing the number of stored elements.
//
// reserve() requests additional storage for future elements.
// Unlike resize(), it does not create new elements.
//
// Expected behavior:
// Capacity increases, but the logical collection size
// remains unchanged.

TEST_F(CollectionTest, ReserveIncreasesCapacityNotSize)
{
    // Populate the collection with five values.
    add_entries(5);

    // Confirm that the initial element count is correct.
    ASSERT_EQ(collection->size(), 5);

    // Save the original number of stored elements.
    size_t originalSize = collection->size();

    // Save the capacity before requesting more storage.
    size_t originalCapacity = collection->capacity();

    // Request enough storage for ten additional elements
    // beyond the currently allocated capacity.
    collection->reserve(originalCapacity + 10);

    // Confirm that capacity increased.
    EXPECT_GT(collection->capacity(), originalCapacity);

    // Verify that capacity meets or exceeds the request.
    //
    // The implementation may allocate more than requested.
    EXPECT_GE(collection->capacity(), originalCapacity + 10);

    // Confirm that reserve() did not insert new elements.
    EXPECT_EQ(collection->size(), originalSize);
}

// ---------------------------------------------------------
// REQUIRED TEST 11: INVALID INDEX EXCEPTION
// ---------------------------------------------------------

// Purpose:
// Perform negative testing by attempting to access an
// element outside the valid vector index range.
//
// Unlike operator[], the at() function performs bounds
// checking and throws std::out_of_range when an index
// is outside the collection.
//
// Expected behavior:
// An exception is thrown, and the collection size
// remains unchanged.

TEST_F(CollectionTest, AtThrowsOutOfRangeException)
{
    // Add five values to the collection.
    add_entries(5);

    // Confirm the vector contains five elements.
    ASSERT_EQ(collection->size(), 5);

    // Valid indices are 0, 1, 2, 3, and 4.
    //
    // Index 5 is outside the collection's valid range.
    // EXPECT_THROW verifies that the correct exception occurs.
    EXPECT_THROW(collection->at(5), std::out_of_range);

    // Confirm that invalid access did not change the size.
    EXPECT_EQ(collection->size(), 5);
}

// ---------------------------------------------------------
// CUSTOM TEST 12: VERIFY INSERTION ORDER
// POSITIVE TEST
// ---------------------------------------------------------

// Purpose:
// Verify that push_back() stores valid values in the
// same order in which they were inserted.
//
// This positive test uses known values rather than
// randomly generated values.
//
// Using known values makes it possible to verify
// both element count and exact element placement.
//
// Expected behavior:
// The collection contains 10, 20, and 30 in that order.

TEST_F(CollectionTest, PushBackStoresCorrectValues)
{
    // Verify that the vector begins empty.
    ASSERT_TRUE(collection->empty());

    // Insert three known integer values.
    collection->push_back(10);
    collection->push_back(20);
    collection->push_back(30);

    // Confirm that all three values were added.
    ASSERT_EQ(collection->size(), 3);

    // Verify that the first element contains 10.
    EXPECT_EQ(collection->at(0), 10);

    // Verify that the second element contains 20.
    EXPECT_EQ(collection->at(1), 20);

    // Verify that the third element contains 30.
    EXPECT_EQ(collection->at(2), 30);

    // front() should return the first stored value.
    EXPECT_EQ(collection->front(), 10);

    // back() should return the last stored value.
    EXPECT_EQ(collection->back(), 30);
}

// ---------------------------------------------------------
// CUSTOM TEST 13: INVALID ACCESS PRESERVES DATA
// NEGATIVE TEST
// ---------------------------------------------------------

// Purpose:
// Verify that an invalid index access does not modify
// existing collection data.
//
// This test checks more than exception handling by
// verifying the integrity of the original values.
//
// Data integrity is important in secure coding because
// unexpected changes can affect application reliability
// and potentially cause additional errors.
//
// Expected behavior:
// The invalid access throws std::out_of_range.
// The collection retains all three original values
// in their original positions.

TEST_F(CollectionTest, InvalidAccessDoesNotModifyCollection)
{
    // Insert known values so their contents can be verified.
    collection->push_back(10);
    collection->push_back(20);
    collection->push_back(30);

    // Confirm the starting collection contains three elements.
    ASSERT_EQ(collection->size(), 3);

    // Attempt to access index 100.
    //
    // Since the collection contains only three elements,
    // the valid indices are 0, 1, and 2.
    //
    // The at() function should detect this invalid index
    // and throw std::out_of_range.
    EXPECT_THROW(collection->at(100), std::out_of_range);

    // Verify that the unsuccessful access did not
    // increase or decrease the collection size.
    EXPECT_EQ(collection->size(), 3);

    // Confirm the first value remains unchanged.
    EXPECT_EQ(collection->at(0), 10);

    // Confirm the second value remains unchanged.
    EXPECT_EQ(collection->at(1), 20);

    // Confirm the third value remains unchanged.
    EXPECT_EQ(collection->at(2), 30);
}
