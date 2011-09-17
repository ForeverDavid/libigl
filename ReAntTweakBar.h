// ReAntTweakBar is a minimal wrapper for the AntTweakBar library that allows
// "bars" to be saved and load from disk. Changing your existing app that users
// AntTweakBar to use ReAntTweakBar is trivial.
// 
// Many (but not all) variable types are supported. I'll try to keep track them
// here:
//   TW_TYPE_BOOLCPP
//   TW_TYPE_QUAT4F
//   TW_TYPE_COLOR4F
//   TW_TYPE_COLOR3F
//   TW_TYPE_DIR3F
//   TW_TYPE_BOOL32
//   TW_TYPE_INT32
//   TW_TYPE_FLOAT
//   TW_TYPE_DOUBLE
//   and
//   custom TwTypes made with TwDefineEnum
// 
// I'm working on adding the rest on an as-needed basis. Adding a new type only
// requires changes in a few places...
// 
//
// Copyright Alec Jacobson, 2011
//

// This allows the user to have a non-global, static installation of
// AntTweakBar
#ifdef STATIC_ANTTWEAKBAR
#  include "AntTweakBar.h"
#else
#  include <AntTweakBar.h>
#endif

#include <vector>
#include <string>

namespace igl
{

  TwType ReTwDefineEnum(const char *name, const TwEnumVal *enumValues, unsigned int nbValues);
  
  struct ReTwRWItem
  {
    const char * name;
    TwType type;
    void * var;
    ReTwRWItem(
      const char * name,
      TwType type, 
      void *var)
    {
      this->name = name;
      this->type = type;
      this->var = var;
    }
  };
  
  struct ReTwCBItem
  {
    const char * name;
    TwSetVarCallback setCallback;
    TwGetVarCallback getCallback;
    void * clientData;
    TwType type;
    ReTwCBItem(
      const char * name,
      TwType type, 
      TwSetVarCallback setCallback,
      TwGetVarCallback getCallback,
      void * clientData)
    {
      this->name = name;
      this->type = type;
      this->setCallback = setCallback;
      this->getCallback = getCallback;
      this->clientData = clientData;
    }
  };
  
  class ReTwBar
  {
    // VARIABLES
    // Should be private, but seeing as I'm not going to implement all of the
    // AntTweakBar public functions right away, I'll expose this so that at
    // anytime AntTweakBar functions can be called directly on the bar
    public:
      TwBar * bar;
    private:
      std::vector<ReTwRWItem> rw_items;
      std::vector<ReTwCBItem> cb_items;
  
    // WRAPPERS FOR ANTTWEAKBAR FUNCTIONS 
    public:
      void TwNewBar(const char *barName);
      int TwAddVarRW(
        const char *name, 
        TwType type, 
        void *var, 
        const char *def,
        const bool record=true);
      int TwAddVarCB(
        const char *name, 
        TwType type, 
        TwSetVarCallback setCallback, 
        TwGetVarCallback getCallback, 
        void *clientData, 
        const char *def,
        const bool record=true);
      // Wrappers for convenience (not recorded, just passed on)
      int TwAddVarRO(const char *name, TwType type, void *var, const char *def);
      int TwAddButton(
        const char *name, 
        TwButtonCallback buttonCallback, 
        void *clientData, 
        const char *def);
      int TwSetParam(
        const char *varName, 
        const char *paramName, 
        TwParamValueType paramValueType, 
        unsigned int inValueCount, 
        const void *inValues);
      int TwGetParam(
        const char *varName, 
        const char *paramName, 
        TwParamValueType paramValueType, 
        unsigned int outValueMaxCount, 
        void *outValues);
      int TwRefreshBar();
  
  
    // IO FUNCTIONS
    public:
      // Save current items to file
      // Input:
      //   file_name  name of file to save data to, can be null which means print
      //   to stdout
      // Return:
      //   true only if there were no (fatal) errors
      bool save(const char *file_name);
      std::string get_value_as_string(
        void * var, 
        TwType type);
      // Load into current items from file
      // Input:
      //   file_name  name of input file to load
      // Return:
      //   true only if there were no (fatal) errors
      bool load(const char *file_name);
      // Get TwType from string
      // Input
      //   type_str  string of type 
      // Output
      //   type  TwType converted from string
      // Returns
      //   true only if string matched a valid type
      bool type_from_string(const char *type_str, TwType & type);
      // I realize that I mixing std::string and const char * all over the place.
      // What can you do...
      bool set_value_from_string(
        const char * name, 
        TwType type, 
        const char * value_str);
  };

};

// List of TwBar functions
//TW_API TwBar *      TW_CALL TwNewBar(const char *barName);
//TW_API int          TW_CALL TwDeleteBar(TwBar *bar);
//TW_API int          TW_CALL TwDeleteAllBars();
//TW_API int          TW_CALL TwSetTopBar(const TwBar *bar);
//TW_API TwBar *      TW_CALL TwGetTopBar();
//TW_API int          TW_CALL TwSetBottomBar(const TwBar *bar);
//TW_API TwBar *      TW_CALL TwGetBottomBar();
//TW_API const char * TW_CALL TwGetBarName(TwBar *bar);
//TW_API int          TW_CALL TwGetBarCount();
//TW_API TwBar *      TW_CALL TwGetBarByIndex(int barIndex);
//TW_API TwBar *      TW_CALL TwGetBarByName(const char *barName);
//TW_API int          TW_CALL TwRefreshBar(TwBar *bar);
//
//TW_API int      TW_CALL TwAddVarRW(TwBar *bar, const char *name, TwType type, void *var, const char *def);
//TW_API int      TW_CALL TwAddVarRO(TwBar *bar, const char *name, TwType type, const void *var, const char *def);
//TW_API int      TW_CALL TwAddVarCB(TwBar *bar, const char *name, TwType type, TwSetVarCallback setCallback, TwGetVarCallback getCallback, void *clientData, const char *def);
//TW_API int      TW_CALL TwAddButton(TwBar *bar, const char *name, TwButtonCallback callback, void *clientData, const char *def);
//TW_API int      TW_CALL TwAddSeparator(TwBar *bar, const char *name, const char *def);
//TW_API int      TW_CALL TwRemoveVar(TwBar *bar, const char *name);
//TW_API int      TW_CALL TwRemoveAllVars(TwBar *bar);

// Implementation

#include <cstdio>
#include <sstream>
#include <map>

#define MAX_CB_VAR_SIZE 10
// Max line size for reading files
#define MAX_LINE 1000
#define MAX_WORD 100

// GLOBAL WRAPPERS
namespace igl
{
  std::map<TwType,std::pair<const char *,std::vector<TwEnumVal> > > ReTw_custom_types;
}

TwType igl::ReTwDefineEnum(
  const char *name, 
  const TwEnumVal *enumValues, 
  unsigned int nbValues)
{
  // copy enum valus into vector
  std::vector<TwEnumVal> enum_vals;
  enum_vals.resize(nbValues);
  for(unsigned int j = 0; j<nbValues;j++)
  {
    enum_vals[j] = enumValues[j];
  }
  TwType type = TwDefineEnum(name,enumValues,nbValues);

  ReTw_custom_types[type] = 
    std::pair<const char *,std::vector<TwEnumVal> >(name,enum_vals);
  return type;
}

namespace igl
{
  struct ReTwTypeString
  {
    TwType type;
    const char * type_str;
  };

  #define RETW_NUM_DEFAULT_TYPE_STRINGS 23
  ReTwTypeString ReTwDefaultTypeStrings[RETW_NUM_DEFAULT_TYPE_STRINGS] = 
  {
    {TW_TYPE_UNDEF,"TW_TYPE_UNDEF"},
    {TW_TYPE_BOOLCPP,"TW_TYPE_BOOLCPP"},
    {TW_TYPE_BOOL8,"TW_TYPE_BOOL8"},
    {TW_TYPE_BOOL16,"TW_TYPE_BOOL16"},
    {TW_TYPE_BOOL32,"TW_TYPE_BOOL32"},
    {TW_TYPE_CHAR,"TW_TYPE_CHAR"},
    {TW_TYPE_INT8,"TW_TYPE_INT8"},
    {TW_TYPE_UINT8,"TW_TYPE_UINT8"},
    {TW_TYPE_INT16,"TW_TYPE_INT16"},
    {TW_TYPE_UINT16,"TW_TYPE_UINT16"},
    {TW_TYPE_INT32,"TW_TYPE_INT32"},
    {TW_TYPE_UINT32,"TW_TYPE_UINT32"},
    {TW_TYPE_FLOAT,"TW_TYPE_FLOAT"},
    {TW_TYPE_DOUBLE,"TW_TYPE_DOUBLE"},
    {TW_TYPE_COLOR32,"TW_TYPE_COLOR32"},
    {TW_TYPE_COLOR3F,"TW_TYPE_COLOR3F"},
    {TW_TYPE_COLOR4F,"TW_TYPE_COLOR4F"},
    {TW_TYPE_CDSTRING,"TW_TYPE_CDSTRING"},
    {TW_TYPE_STDSTRING,"TW_TYPE_STDSTRING"},
    {TW_TYPE_QUAT4F,"TW_TYPE_QUAT4F"},
    {TW_TYPE_QUAT4D,"TW_TYPE_QUAT4D"},
    {TW_TYPE_DIR3F,"TW_TYPE_DIR3F"},
    {TW_TYPE_DIR3D,"TW_TYPE_DIR3D"}
  };
}


// BAR WRAPPERS
void igl::ReTwBar::TwNewBar(const char *barName)
{
  // double colon without anything in front of it means look for this in the
  // global namespace... I hope...
  this->bar = ::TwNewBar(barName);
}

int igl::ReTwBar::TwAddVarRW(
  const char *name, 
  TwType type, 
  void *var, 
  const char *def,
  const bool record)
{
  int ret = ::TwAddVarRW(this->bar,name,type,var,def);
  if(ret && record)
  {
    rw_items.push_back(ReTwRWItem(name,type,var));
  }
  return ret;
}

int igl::ReTwBar::TwAddVarCB(
  const char *name, 
  TwType type, 
  TwSetVarCallback setCallback, 
  TwGetVarCallback getCallback, 
  void *clientData, 
  const char *def,
  const bool record)
{
  int ret = 
    ::TwAddVarCB(this->bar,name,type,setCallback,getCallback,clientData,def);
  if(ret && record)
  {
    cb_items.push_back(ReTwCBItem(name,type,setCallback,getCallback,clientData));
  }
  return ret;
}

int igl::ReTwBar::TwAddVarRO(
  const char *name, 
  TwType type, 
  void *var, 
  const char *def)
{
  int ret = ::TwAddVarRO(this->bar,name,type,var,def);
  // Read only variables are not recorded
  //if(ret)
  //{
  //  rw_items.push_back(ReTwRWItem(name,type,var));
  //}
  return ret;
}

int igl::ReTwBar::TwAddButton(
  const char *name, 
  TwButtonCallback buttonCallback, 
  void *clientData, 
  const char *def)
{
  int ret = 
    ::TwAddButton(this->bar,name,buttonCallback,clientData,def);
  // buttons are not recorded
  //if(ret)
  //{
  //  cb_items.push_back(ReTwCBItem(name,type,setCallback,getCallback,clientData));
  //}
  return ret;
}

int igl::ReTwBar::TwSetParam(
  const char *varName, 
  const char *paramName, 
  TwParamValueType paramValueType, 
  unsigned int inValueCount, 
  const void *inValues)
{
  // For now just pass these along
  return 
    ::TwSetParam(
      this->bar,
      varName,
      paramName,
      paramValueType,
      inValueCount,
      inValues);
}

int igl::ReTwBar::TwGetParam(
  const char *varName, 
  const char *paramName, 
  TwParamValueType paramValueType, 
  unsigned int outValueMaxCount,
  void *outValues)
{
  return 
    ::TwGetParam(
      this->bar,
      varName,
      paramName,
      paramValueType,
      outValueMaxCount,
      outValues);
}

int igl::ReTwBar::TwRefreshBar()
{
  return ::TwRefreshBar(this->bar);
}
bool igl::ReTwBar::save(const char *file_name)
{
  FILE * fp;
  if(file_name == NULL)
  {
    fp = stdout;
  }else
  {
    fp = fopen(file_name,"w");
  }

  if(fp == NULL)
  {
    printf("ERROR: not able to open %s for writing...\n",file_name);
    return false;
  }

  // Print all RW variables
  for(
    std::vector<ReTwRWItem>::iterator it = rw_items.begin(); 
    it != rw_items.end(); 
    it++)
  {
    const char * name = (*it).name;
    TwType type = (*it).type;
    void * var = (*it).var;
    fprintf(fp,"%s: %s\n",
      name,
      get_value_as_string(var,type).c_str());
  }

  char var[MAX_CB_VAR_SIZE];
  // Print all CB variables
  for(
    std::vector<ReTwCBItem>::iterator it = cb_items.begin(); 
    it != cb_items.end(); 
    it++)
  {
    const char * name = it->name;
    TwType type = it->type;
    //TwSetVarCallback setCallback = it->setCallback;
    TwGetVarCallback getCallback = it->getCallback;
    void * clientData = it->clientData;
    // I'm not sure how to do what I want to do. getCallback needs to be sure
    // that it can write to var. So var needs to point to a valid and big
    // enough chunk of memory
    getCallback(var,clientData);
    fprintf(fp,"%s: %s\n",
      name,
      get_value_as_string(var,type).c_str());
  }

  fprintf(fp,"\n");

  if(file_name != NULL)
  {
    fclose(fp);
  }
  // everything succeeded
  return true;
}

std::string igl::ReTwBar::get_value_as_string(
  void * var, 
  TwType type)
{
  std::stringstream sstr;
  switch(type)
  {
    case TW_TYPE_BOOLCPP:
      {
        sstr << "TW_TYPE_BOOLCPP" << " ";
        sstr << *(static_cast<bool*>(var));
        break;
      }
    case TW_TYPE_QUAT4F:
      {
        sstr << "TW_TYPE_QUAT4F" << " ";
        // Q: Why does casting to float* work? shouldn't I have to cast to
        // float**?
        float * q = static_cast<float*>(var);
        sstr << q[0] << " " << q[1] << " " << q[2] << " " << q[3];
        break;
      }
    case TW_TYPE_COLOR4F:
      {
        sstr << "TW_TYPE_COLOR4F" << " ";
        float * c = static_cast<float*>(var);
        sstr << c[0] << " " << c[1] << " " << c[2] << " " << c[3];
        break;
      }
    case TW_TYPE_COLOR3F:
      {
        sstr << "TW_TYPE_COLOR3F" << " ";
        float * c = static_cast<float*>(var);
        sstr << c[0] << " " << c[1] << " " << c[2];
        break;
      }
    case TW_TYPE_DIR3F:
      {
        sstr << "TW_TYPE_DIR3F" << " ";
        float * d = static_cast<float*>(var);
        sstr << d[0] << " " << d[1] << " " << d[2];
        break;
      }
    case TW_TYPE_BOOL32:
      {
        sstr << "TW_TYPE_BOOL32" << " ";
        sstr << *(static_cast<int*>(var));
        break;
      }
    case TW_TYPE_INT32:
      {
        sstr << "TW_TYPE_INT32" << " ";
        sstr << *(static_cast<int*>(var));
        break;
      }
    case TW_TYPE_FLOAT:
      {
        sstr << "TW_TYPE_FLOAT" << " ";
        sstr << *(static_cast<float*>(var));
        break;
      }
    case TW_TYPE_DOUBLE:
      {
        sstr << "TW_TYPE_DOUBLE" << " ";
        sstr << *(static_cast<double*>(var));
        break;
      }
    default:
      {
        std::map<TwType,std::pair<const char *,std::vector<TwEnumVal> > >::iterator iter = 
          ReTw_custom_types.find(type);
        if(iter != ReTw_custom_types.end())
        {
          sstr << (*iter).second.first << " ";
          int enum_val = *(static_cast<int*>(var));
          // try find display name for enum value
          std::vector<TwEnumVal>::iterator eit = (*iter).second.second.begin();
          bool found = false;
          for(;eit<(*iter).second.second.end();eit++)
          {
            if(enum_val == eit->Value)
            {
              sstr << eit->Label;
              found = true;
              break;
            }
          }
          if(!found)
          {
            sstr << "ERROR_ENUM_VALUE_NOT_DEFINED";
          }
        }else
        {
          sstr << "ERROR_TYPE_NOT_SUPPORTED";
        }
        break;
      }
  }
  return sstr.str();
}

bool igl::ReTwBar::load(const char *file_name)
{
  FILE * fp;
  fp = fopen(file_name,"r");

  if(fp == NULL)
  {
    printf("ERROR: not able to open %s for reading...\n",file_name);
    return false;
  }

  // go through file line by line
  char line[MAX_LINE];
  bool still_comments;
  char name[MAX_WORD];
  char type_str[MAX_WORD];
  char value_str[MAX_WORD];


  // line number
  int j = 0;
  bool finished = false;
  while(true)
  {
    // Eat comments
    still_comments = true;
    while(still_comments)
    {
      if(fgets(line,MAX_LINE,fp) == NULL)
      {
        finished = true;
        break;
      }
      // Blank lines and lines that begin with # are comments
      still_comments = (line[0] == '#' || line[0] == '\n');
      j++;
    }
    if(finished)
    {
      break;
    }

    sscanf(line,"%[^:]: %s %[^\n]",name,type_str,value_str);
    //printf("%s: %s %s\n",name, type_str,value_str);

    TwType type;
    if(!type_from_string(type_str,type))
    {
      printf("ERROR: %s type not found... Skipping...\n",type_str);
      continue;
    }
    set_value_from_string(name,type,value_str);

  }

  fclose(fp);
  
  // everything succeeded
  return true;
}

bool igl::ReTwBar::type_from_string(const char *type_str, TwType & type)
{
  // first check default types
  for(int j = 0; j < RETW_NUM_DEFAULT_TYPE_STRINGS; j++)
  {
    if(strcmp(type_str,ReTwDefaultTypeStrings[j].type_str) == 0)
    {
      type = ReTwDefaultTypeStrings[j].type;
      return true;
      break;
    }
  }

  // then check custom types
  std::map<TwType,std::pair<const char *,std::vector<TwEnumVal> > >::iterator iter = 
    ReTw_custom_types.begin();
  for(;iter != ReTw_custom_types.end(); iter++)
  {
    if(strcmp((*iter).second.first,type_str)==0)
    {
      type = (*iter).first;
      return true;
    }
  }
  return false;
}

bool igl::ReTwBar::set_value_from_string(
  const char * name, 
  TwType type, 
  const char * value_str)
{
  void * value;
  // possible value slots
  int i;
  float v;
  double dv;
  float f[4];
  bool b;

  // First try to get value from default types
  switch(type)
  {
    case TW_TYPE_BOOLCPP:
      {
        int ib;
        if(sscanf(value_str," %d",&ib) == 1)
        {
          b = ib!=0;
          value = &b;
        }else
        {
          printf("ERROR: Bad value format...\n");
          return false;
        }
        break;
      }
    case TW_TYPE_QUAT4F:
    case TW_TYPE_COLOR4F:
      {
        if(sscanf(value_str," %f %f %f %f",&f[0],&f[1],&f[2],&f[3]) == 4)
        {
          value = &f;
        }else
        {
          printf("ERROR: Bad value format...\n");
          return false;
        }
        break;
      }
    case TW_TYPE_COLOR3F:
    case TW_TYPE_DIR3F:
      {
        if(sscanf(value_str," %f %f %f",&f[0],&f[1],&f[2]) == 3)
        {
          value = &f;
        }else
        {
          printf("ERROR: Bad value format...\n");
          return false;
        }
        break;
      }
    case TW_TYPE_BOOL32:
    case TW_TYPE_INT32:
      {
        if(sscanf(value_str," %d",&i) == 1)
        {
          value = &i;
        }else
        {
          printf("ERROR: Bad value format...\n");
          return false;
        }
        break;
      }
    case TW_TYPE_FLOAT:
      {
        if(sscanf(value_str," %f",&v) == 1)
        {
          value = &v;
        }else
        {
          printf("ERROR: Bad value format...\n");
          return false;
        }
        break;
      }
    case TW_TYPE_DOUBLE:
      {
        if(sscanf(value_str," %lf",&dv) == 1)
        {
          value = &dv;
        }else
        {
          printf("ERROR: Bad value format...\n");
          return false;
        }
        break;
      }
    default:
      // Try to find type in custom enum types
      std::map<TwType,std::pair<const char *,std::vector<TwEnumVal> > >::iterator iter = 
        ReTw_custom_types.find(type);
      if(iter != ReTw_custom_types.end())
      {
        std::vector<TwEnumVal>::iterator eit = (*iter).second.second.begin();
        bool found = false;
        for(;eit<(*iter).second.second.end();eit++)
        {
          if(strcmp(value_str,eit->Label) == 0)
          {
            i = eit->Value;
            value = &i;
            found = true;
            break;
          }
        }
        if(!found)
        {
          printf("ERROR_ENUM_VALUE_NOT_DEFINED");
        }
      }else
      {
        printf("ERROR_TYPE_NOT_SUPPORTED\n");
      }

      break;
  }


  // Find variable based on name
  // First look in RW items
  bool item_found = false;
  for(
    std::vector<ReTwRWItem>::iterator it = rw_items.begin(); 
    it != rw_items.end(); 
    it++)
  {
    if(strcmp(it->name,name) == 0)
    {
      void * var = it->var;
      switch(type)
      {
        case TW_TYPE_BOOLCPP:
          {
            bool * bvar = static_cast<bool*>(var);
            bool * bvalue = static_cast<bool*>(value);
            *bvar = *bvalue;
            break;
          }
        case TW_TYPE_QUAT4F:
        case TW_TYPE_COLOR4F:
          {
            float * fvar = static_cast<float*>(var);
            float * fvalue = static_cast<float*>(value);
            fvar[0] = fvalue[0];
            fvar[1] = fvalue[1];
            fvar[2] = fvalue[2];
            fvar[3] = fvalue[3];
            break;
          }
        case TW_TYPE_COLOR3F:
        case TW_TYPE_DIR3F:
          {
            float * fvar = static_cast<float*>(var);
            float * fvalue = static_cast<float*>(value);
            fvar[0] = fvalue[0];
            fvar[1] = fvalue[1];
            fvar[2] = fvalue[2];
            break;
          }
        case TW_TYPE_BOOL32:
        case TW_TYPE_INT32:
          {
            int * ivar = static_cast<int*>(var);
            int * ivalue = static_cast<int*>(value);
            *ivar = *ivalue;
            break;
          }
        case TW_TYPE_FLOAT:
          {
            float * fvar = static_cast<float*>(var);
            float * fvalue = static_cast<float*>(value);
            *fvar = *fvalue;
            break;
          }
        case TW_TYPE_DOUBLE:
          {
            double * dvar =   static_cast<double*>(var);
            double * fvalue = static_cast<double*>(value);
            *dvar = *fvalue;
            break;
          }
        default:
          // Try to find type in custom enum types
          std::map<TwType,std::pair<const char *,std::vector<TwEnumVal> > >::iterator iter = 
            ReTw_custom_types.find(type);
          if(iter != ReTw_custom_types.end())
          {
            int * ivar = static_cast<int*>(var);
            std::vector<TwEnumVal>::iterator eit = (*iter).second.second.begin();
            bool found = false;
            for(;eit<(*iter).second.second.end();eit++)
            {
              if(strcmp(value_str,eit->Label) == 0)
              {
                *ivar = eit->Value;
                found = true;
                break;
              }
            }
            if(!found)
            {
              printf("ERROR_ENUM_VALUE_NOT_DEFINED");
            }
          }else
          {
            printf("ERROR_TYPE_NOT_SUPPORTED\n");
          }
          break;
      }
      item_found = true;
      break;
    }
  }

  // Try looking in CB items
  if(!item_found)
  {
    for(
      std::vector<ReTwCBItem>::iterator it = cb_items.begin(); 
      it != cb_items.end(); 
      it++)
    {
      if(strcmp(it->name,name) == 0)
      {
        it->setCallback(value,it->clientData);
        item_found = true;
        break;
      }
    }
  }

  if(!item_found)
  {
    printf("ERROR: item not found\n");
  }
  return true;
}