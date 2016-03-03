#ifndef PHP_UTILS_H
#define PHP_UTILS_H

#define PHP_UTILS_EXTNAME  "Utils"
#define PHP_UTILS_VERSION   "0.1"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

extern "C" {
#include "php.h"
}

extern zend_module_entry utils_module_entry;
#define phpext_uuid_ptr &utils_module_entry;
PHP_MINIT_FUNCTION(utils);
//PHP_MSUTDOWN_FUNCTION(utils);
PHP_RINIT_FUNCTION(utils);
PHP_RSHUTDOWN_FUNCTION(utils);
PHP_MINFO_FUNCTION(utils);

PHP_METHOD(uuid, __construct);
PHP_METHOD(uuid, setMachine);
PHP_METHOD(uuid, setEpoch);
PHP_METHOD(uuid, getGenerate);

#endif /* PHP_UTILS_H */
