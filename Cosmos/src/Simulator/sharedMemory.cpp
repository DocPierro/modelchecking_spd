#include <iostream>

#include "sharedMemory.hpp"
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>

// namespace bi = boost::interprocess;
using namespace boost::interprocess;
// Define an STL compatible allocator of ints that allocates from the
// managed_shared_memory. This allocator will allow placing containers in the
// segment
typedef allocator<int, managed_shared_memory::segment_manager> ShmemAllocator;

// Alias a vector that uses the previous STL-like allocator so that allocates
// its values from the segment
typedef vector<int, ShmemAllocator> MyVector;

void build_segment() {
  struct shm_remove {
    shm_remove() { shared_memory_object::remove("MySharedMemory"); }
    ~shm_remove() { shared_memory_object::remove("MySharedMemory"); }
  } remover;

  // Create a new segment with given name and size
  managed_shared_memory segment(create_only, "MySharedMemory", 65536);

  // Initialize shared memory STL-compatible allocator
  const ShmemAllocator alloc_inst(segment.get_segment_manager());

  // Construct a vector named "MyVector" in shared memory with argument
  // alloc_inst
  MyVector *myvector = segment.construct<MyVector>("MyVector")(alloc_inst);

  for (int i = 0; i < 100; ++i) // Insert data in the vector
    myvector->push_back(i);
}

void retrive_segment() {
  // Open the managed segment
  managed_shared_memory segment(open_only, "MySharedMemory");

  // Find the vector using the c-string name
  MyVector *myvector = segment.find<MyVector>("MyVector").first;

  // Use vector in reverse order
  for (auto it = myvector->begin(); it != myvector->end(); ++it) {
    std::cerr << "printvect" << *it << std::endl;
  }

  // When done, destroy the vector from the segment
  segment.destroy<MyVector>("MyVector");
}
