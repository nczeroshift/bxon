
#ifndef BXON_H_
#define BXON_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif
    
typedef enum{
    // Collection default type
    BXON_OBJECT = 0,
    
    // Native types
    BXON_STRING = 1,
    BXON_BOOLEAN = 2,
    BXON_INT = 3,
    BXON_LONG = 4,
    BXON_FLOAT = 4,
    BXON_DOUBLE = 5,
    BXON_BYTE = 6,
    
    // Flags
    BXON_MAP = 64,
    BXON_ARRAY = 128,
} bxon_types;

typedef enum{
    BXON_ERROR_NONE = 0,
    BXON_ERROR_ARRAY_OR_MAP_MISSING = 1
} bxon_error;

#define BXON_MASK_TYPE 0x3F
#define BXON_MASK_FLAG 0xC0

struct bxon_context;

typedef uint32_t (bxon_read)    (struct bxon_context * context, int32_t size, uint8_t * d);
typedef uint32_t (bxon_write)   (struct bxon_context * context, int32_t size, uint8_t * d);
typedef uint64_t (bxon_seek)    (struct bxon_context * context, int64_t offset);
typedef uint64_t (bxon_tell)    (struct bxon_context * context);

struct bxon_context{
    bxon_read * read;
    bxon_write * write;
    bxon_seek * seek;
    bxon_tell * tell;
    uint32_t error;
    void * data;
};

struct bxon_header{
    uint8_t type;
    uint64_t length;
};

struct bxon_object{
    struct bxon_header header;
    struct bxon_context * context;
    void * data;
};

struct bxon_data_array{
    uint32_t initCapacity;
    uint32_t capacity;
    uint32_t size;
    void * objects;
    int64_t * offset;
};
    
struct bxon_data_map{
    uint32_t initCapacity;
    uint32_t capacity;
    uint32_t size;
    char ** keys;
    void ** objects;
    int64_t * offset;
};

void                    bxon_release    (struct bxon_object ** obj);

uint8_t                 bxon_is_map     (struct bxon_object * obj);
uint8_t                 bxon_is_array   (struct bxon_object * obj);
uint8_t                 bxon_get_type   (struct bxon_object * obj);

struct bxon_object *    bxon_new_int    (int32_t value);
struct bxon_object *    bxon_new_long   (int64_t value);
struct bxon_object *    bxon_new_bool   (uint8_t value);
struct bxon_object *    bxon_new_float  (float value);
struct bxon_object *    bxon_new_double (double value);
struct bxon_object *    bxon_new_byte   (uint8_t byte);
struct bxon_object *    bxon_new_string (const char * string);

int32_t                 bxon_get_int    (struct bxon_object * obj);
int64_t                 bxon_get_long   (struct bxon_object * obj);
uint8_t                 bxon_get_bool   (struct bxon_object * obj);
float                   bxon_get_float  (struct bxon_object * obj);
double                  bxon_get_double (struct bxon_object * obj);
uint8_t                 bxon_get_byte   (struct bxon_object * obj);
int32_t                 bxon_get_string (struct bxon_object * obj, char ** string);

struct bxon_object *    bxon_array_new          (uint8_t nativeType, uint32_t initCapacity);
uint32_t                bxon_array_size         (struct bxon_object * obj);
uint32_t                bxon_array_push         (struct bxon_object * obj, struct bxon_object * elem);
struct bxon_object *    bxon_array_get_object   (struct bxon_object * obj, uint32_t index);

struct bxon_object *    bxon_map_new        (uint32_t initCapacity);
uint32_t                bxon_map_size       (struct bxon_object * obj);
uint32_t                bxon_map_put        (struct bxon_object * obj, const char * key, struct bxon_object * elem);
struct bxon_object *    bxon_map_get_object (struct bxon_object * obj, const char * key);
const char *            bxon_map_get_key    (struct bxon_object * obj, int32_t index);

uint64_t                bxon_write_object   (struct bxon_object * obj, struct bxon_context * ctx);

#ifdef __cplusplus
}
#endif

#endif
