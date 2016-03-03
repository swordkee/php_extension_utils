#include "php_utils.h"
#include "uuid.h"
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include <iostream>

using namespace std;

zend_object_handlers uuid_object_handlers;
zend_class_entry *uuid_ce;

struct uuid_object {
    zend_object std;
    uuid *u_id;
};


void uuid_free_storage( void *object TSRMLS_DC ){

    uuid_object *obj = (uuid_object *)object;

    delete obj->u_id;

    zend_hash_destroy(obj->std.properties);
    FREE_HASHTABLE(obj->std.properties);

    efree(obj);

}

#if PHP_MAJOR_VERSION >= 7
zend_object* uuid_create_handler(zend_class_entry *type TSRMLS_DC)
{
    zval *tmp;

    uuid_object *obj = (uuid_object *)emalloc(sizeof(uuid_object));
    memset(obj, 0, sizeof(uuid_object));
    obj->std.ce = type;

    ALLOC_HASHTABLE(obj->std.properties);
    zend_hash_init(obj->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
    obj->std.handlers = (zend_object_handlers *) &uuid_object_handlers;

    return &obj->std;
}
#else
zend_object_value uuid_create_handler( zend_class_entry *type TSRMLS_DC ){

    zend_object_value retval;

    uuid_object *obj = (uuid_object *)emalloc( sizeof(uuid_object) );
    memset( obj, 0, sizeof(uuid_object) );
    obj->std.ce = type;

    ALLOC_HASHTABLE( obj->std.properties );
    zend_hash_init( obj->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0 );
#if PHP_VERSION_ID < 50399
    zval *tmp;
    zend_hash_copy(obj->std.properties,&type->default_properties,
                   (copy_ctor_func_t) zval_add_ref, (void *) & tmp, sizeof (void *));
#else
    object_properties_init(&obj->std, type);
#endif

    retval.handle = zend_objects_store_put( obj, NULL, uuid_free_storage, NULL TSRMLS_CC );
    retval.handlers = &uuid_object_handlers;

    return retval;

}
#endif




PHP_METHOD( uuid, __construct ){
    uuid *u_id = NULL;
    u_id= new uuid();
#if PHP_MAJOR_VERSION >= 7
    uuid_object *obj = (uuid_object *)Z_OBJ_P(getThis());
#else
    uuid_object *obj = (uuid_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
#endif
    obj->u_id = u_id;
}


PHP_METHOD( uuid, setEpoch ){
    zval* epoch;
    uuid *u_id;
    if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &epoch) == FAILURE ){
        RETURN_FALSE;
    }
#if PHP_MAJOR_VERSION >= 7
    uuid_object *obj = (uuid_object *)Z_OBJ_P(getThis());
#else
    uuid_object *obj = (uuid_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
#endif
    u_id = obj->u_id;
    if (u_id != NULL) {
        switch(Z_TYPE_P(epoch)){
        case IS_LONG:
            php_printf("Long: %ld\n", (uint64_t)Z_LVAL_P(epoch));
            if (Z_LVAL_P (epoch) < 0)
                RETURN_FALSE;
            u_id->set_epoch((uint64_t)Z_LVAL_P(epoch));
            break;
        case IS_DOUBLE:
            php_printf("Double: %ld\n", (uint64_t)Z_DVAL_P(epoch));
            if (Z_DVAL_P (epoch) < 0.0)
                RETURN_FALSE;
            u_id->set_epoch((uint64_t)Z_DVAL_P(epoch));
            break;
        case IS_STRING:
            uint64_t val;
            char *end;

            errno = 0;
            val = (uint64_t) strtoull (Z_STRVAL_P (epoch), &end, 0);

            if (*end || (errno == ERANGE && val == UINT64_MAX) || (errno != 0 && val == 0)) {
                RETURN_FALSE;
            }
            php_printf("String: %ld\n", val);
            u_id->set_epoch(val);
            break;
        default:
            RETURN_FALSE;
            break;
        }
    }

    RETURN_NULL();
}
PHP_METHOD( uuid, setMachine ){
    uuid *u_id;
    long machine;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &machine) == FAILURE) {
        RETURN_FALSE;
    }

#if PHP_MAJOR_VERSION >= 7
    uuid_object *obj = (uuid_object *)Z_OBJ_P(getThis());
#else
    uuid_object *obj = (uuid_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
#endif
    u_id = obj->u_id;
    if (u_id != NULL) {
        php_printf("Long: %ld\n", (machine));
        u_id->set_machine(((uint32_t)machine));
        RETURN_TRUE;
    }
    RETURN_NULL();
}
PHP_METHOD( uuid, getGenerate ){
    uuid *u_id;

#if PHP_MAJOR_VERSION >= 7
    uuid_object *obj = (uuid_object *)Z_OBJ_P(getThis());
#else
    uuid_object *obj = (uuid_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
#endif
    u_id = obj->u_id;
    if (u_id != NULL) {
        RETURN_LONG(u_id->generate());
    }
    RETURN_NULL();
}
ZEND_BEGIN_ARG_INFO_EX(ai_uuid_setEpoch, 0, 0, 1)
ZEND_ARG_INFO(0, epoch)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_uuid_setmachine, 0, 0, 1)
ZEND_ARG_INFO(0, machine)
ZEND_END_ARG_INFO()

const zend_function_entry uuid_methods[] = {
    PHP_ME(uuid,  __construct,  NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(uuid,  setEpoch,     ai_uuid_setEpoch, ZEND_ACC_PUBLIC)
    PHP_ME(uuid,  setMachine,   ai_uuid_setmachine, ZEND_ACC_PUBLIC)
    PHP_ME(uuid,  getGenerate,  NULL, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};

PHP_MINIT_FUNCTION( utils ){

    zend_class_entry ce;
    INIT_CLASS_ENTRY( ce, "uuid", uuid_methods );
    uuid_ce = zend_register_internal_class( &ce TSRMLS_CC );
    uuid_ce->create_object = uuid_create_handler;
    memcpy(
                &uuid_object_handlers,
                zend_get_std_object_handlers(),
                sizeof(zend_object_handlers)
                );
    uuid_object_handlers.clone_obj = NULL;
    return SUCCESS;

}
PHP_MINFO_FUNCTION(utils)
{
    php_info_print_table_start();
    php_info_print_table_header(2, PHP_UTILS_EXTNAME, "enabled");
    php_info_print_table_row(2, "Version", PHP_UTILS_VERSION);
    php_info_print_table_end();
}
zend_module_entry utils_module_entry = {
    #if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
    #endif
    PHP_UTILS_EXTNAME,
    NULL,        /* Functions */
    PHP_MINIT(utils),        /* MINIT */
    NULL,        /* MSHUTDOWN */
    NULL,        /* RINIT */
    NULL,        /* RSHUTDOWN */
    NULL,        /* MINFO */
    #if ZEND_MODULE_API_NO >= 20010901
    PHP_UTILS_VERSION,
    #endif
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_UTILS
extern "C" {
ZEND_GET_MODULE(utils)
}
#endif
