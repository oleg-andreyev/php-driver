#include "php_cassandra.h"

zend_class_entry *cassandra_prepared_statement_ce = NULL;

ZEND_EXTERN_MODULE_GLOBALS(cassandra)

PHP_METHOD(PreparedStatement, __construct)
{
}

static zend_function_entry cassandra_prepared_statement_methods[] = {
  PHP_ME(PreparedStatement, __construct, NULL, ZEND_ACC_PRIVATE | ZEND_ACC_CTOR)
  PHP_FE_END
};

static zend_object_handlers cassandra_prepared_statement_handlers;

static HashTable *
php_cassandra_prepared_statement_properties(zval *object TSRMLS_DC)
{
  HashTable *props = zend_std_get_properties(object TSRMLS_CC);

  return props;
}

static int
php_cassandra_prepared_statement_compare(zval *obj1, zval *obj2 TSRMLS_DC)
{
  if (Z_OBJCE_P(obj1) != Z_OBJCE_P(obj2))
    return 1; /* different classes */

  return Z_OBJ_HANDLE_P(obj1) != Z_OBJ_HANDLE_P(obj1);
}

static void
php_cassandra_prepared_statement_free(php5to7_zend_object_free *object TSRMLS_DC)
{
  cassandra_statement *self = PHP5TO7_ZEND_OBJECT_GET(statement, object);

  if (self->prepared)
    cass_prepared_free(self->prepared);

  zend_object_std_dtor(&self->zval TSRMLS_CC);
  PHP5TO7_MAYBE_EFREE(self);
}

static php5to7_zend_object
php_cassandra_prepared_statement_new(zend_class_entry *ce TSRMLS_DC)
{
  cassandra_statement *self =
      PHP5TO7_ZEND_OBJECT_ECALLOC(statement, ce);

  self->type = CASSANDRA_PREPARED_STATEMENT;
  self->prepared = NULL;

  PHP5TO7_ZEND_OBJECT_INIT_EX(statement, prepared_statement, self, ce);
}

void cassandra_define_PreparedStatement(TSRMLS_D)
{
  zend_class_entry ce;

  INIT_CLASS_ENTRY(ce, "Cassandra\\PreparedStatement", cassandra_prepared_statement_methods);
  cassandra_prepared_statement_ce = zend_register_internal_class(&ce TSRMLS_CC);
  zend_class_implements(cassandra_prepared_statement_ce TSRMLS_CC, 1, cassandra_statement_ce);
  cassandra_prepared_statement_ce->ce_flags     |= PHP5TO7_ZEND_ACC_FINAL;
  cassandra_prepared_statement_ce->create_object = php_cassandra_prepared_statement_new;

  memcpy(&cassandra_prepared_statement_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
  cassandra_prepared_statement_handlers.get_properties  = php_cassandra_prepared_statement_properties;
  cassandra_prepared_statement_handlers.compare_objects = php_cassandra_prepared_statement_compare;
}
