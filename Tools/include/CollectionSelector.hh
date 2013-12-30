#ifndef VECBOS_COLLECTIONSELECTOR_HH
#define VECBOS_COLLECTIONSELECTOR_HH

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

namespace vecbos {

  template <class T>
  class CollectionSelector {
  public:
    
    /// objects collection
    typedef std::vector<T> collection;

    /// default constructor
    CollectionSelector();
    /// constructor from an input collection
    CollectionSelector(collection input);
    /// destructor
    ~CollectionSelector() { };

    /// configure from text file
    void configure(std::string cfg);

    /// selected objects
    virtual collection output() { return output_; }

    /// return true only if there is at least one object passing the selection
    bool filter();

  protected:
    /// input collection
    collection input_;
    /// config file
    std::string cfg_;
    /// filter output
    bool filter_;
    /// algorithm and working point
    std::string algo_, wp_;
    /// output collection
    collection output_;
    /// cached output
    bool cached_;
  };

  template <class T>
  CollectionSelector<T>::CollectionSelector() :
    filter_(false), cfg_(""), cached_(false)
  { 
    input_.clear();
    algo_ = std::string("none");
    wp_ = std::string("none");
    output_.clear();
  }
  
  template <class T>
  CollectionSelector<T>::CollectionSelector(collection input) :
    input_(input), filter_(false), cfg_(""), cached_(false)
  { 
    algo_ = std::string("none");
    wp_ = std::string("none");
    output_.clear();
  }
  
  template <class T>
  void CollectionSelector<T>::configure(std::string cfg) {
    cfg_ = cfg;
    std::ifstream cfgfile(cfg_.c_str());
    if(!cfgfile.good()) {
      std::cout << "Error!   Unable to open the cfg file " << cfg_ << std::endl;
      return;
    } else {
      std::string parameter;
      std::string val;
      while (1) {
	cfgfile >> parameter >> val;
	if(parameter.compare("algo")==0) algo_ = val;
	if(parameter.compare("wp")==0) wp_ = val;
      }
      cfgfile.close();    
      if(algo_.compare("none")==0 || wp_.compare("none")==0) {
	std::cout << "Warning! algo or wp not set in the cfg file " 
		  << cfg_ << std::endl;
	return;
      }
    }
  }

  template <class T>
  bool CollectionSelector<T>::filter() {
    if(!cached_) {
      output_ = output();
      cached_ = true;
    }
    return (output_.size() > 0);
  }


}

#endif
