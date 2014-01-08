#ifndef VECBOS_COLLECTIONPTR_CLEANER_HH
#define VECBOS_COLLECTIONPTR_CLEANER_HH

#include <vector>

namespace vecbos {

  template <class T>
  class CollectionPtrCleaner {
  public:

    /// collection of pointers of objects (typically candidates)
    typedef std::vector<T*> collectionPtr;

    /// default constructor
    CollectionPtrCleaner(collectionPtr* collection) :
      collection_(collection) { }
    /// destructor
    ~CollectionPtrCleaner() { };

    /// cleaner
    void clean();

  private:
    collectionPtr * collection_;
  };

  
  template <class T>
  void CollectionPtrCleaner<T>::clean() {
    if(collection_->size() > 0) {
      while(!collection_->empty()) {
	delete collection_->back();
       	collection_->pop_back();
      }
    }
    collection_->clear();
  }

} // namespace vecbos

#endif
