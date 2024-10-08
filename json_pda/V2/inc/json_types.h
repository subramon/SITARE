#include <stdbool.h>
typedef enum _jtype_t { 
  j_undefined,
  j_null,
  j_boolean,
  j_number,
  j_string,
  j_array,
  j_object,
} jtype_t;

// forward declaration
struct _obj_elem_t;
struct _json_t;

typedef struct _json_t {
  jtype_t jtype;
  union _Data { 
    bool bval;
    double num_val;
    char *str_val;
    struct _json_t *arr_val;
    struct _obj_elem_t *obj_val;
  } data;
} json_t;


typedef struct _obj_elem_t {
  char *key;
  json_t *val;
} obj_elem_t;
