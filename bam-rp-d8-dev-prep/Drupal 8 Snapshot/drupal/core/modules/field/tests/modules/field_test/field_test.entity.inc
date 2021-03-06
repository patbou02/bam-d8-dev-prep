<?php

/**
 * @file
 * Defines an entity type.
 */

use Drupal\Core\Entity\EntityInterface;
use Drupal\field_test\Plugin\Core\Entity\TestEntity;

/**
 * Implements hook_entity_info().
 */
function field_test_entity_info() {
  // If requested, clear the field cache while this is being called. See
  // Drupal\field\Tests\FieldInfoTest::testFieldInfoCache().
  if (Drupal::state()->get('field_test.clear_info_cache_in_hook_entity_info')) {
    field_info_cache_clear();
  }
}

/**
 * Implements hook_entity_info_alter().
 */
function field_test_entity_info_alter(&$entity_info) {
  foreach (field_test_entity_info_translatable() as $entity_type => $translatable) {
    $entity_info[$entity_type]['translatable'] = $translatable;
  }
}

/**
 * Helper function to enable entity translations.
 */
function field_test_entity_info_translatable($entity_type = NULL, $translatable = NULL) {
  drupal_static_reset('field_has_translation_handler');
  $stored_value = &drupal_static(__FUNCTION__, array());
  if (isset($entity_type)) {
    $stored_value[$entity_type] = $translatable;
    entity_info_cache_clear();
  }
  return $stored_value;
}

/**
 * Form combining two separate entities.
 */
function field_test_entity_nested_form($form, &$form_state, $entity_1, $entity_2) {
  // First entity.
  foreach (array('id', 'type') as $key) {
    $form[$key] = array(
      '#type' => 'value',
      '#value' => $entity_1->$key->value,
    );
  }
  $form_state['form_display'] = entity_get_form_display($entity_1->entityType(), $entity_1->bundle(), 'default');
  field_attach_form($entity_1, $form, $form_state);

  // Second entity.
  $form['entity_2'] = array(
    '#type' => 'details',
    '#title' => t('Second entity'),
    '#tree' => TRUE,
    '#parents' => array('entity_2'),
    '#weight' => 50,
  );
  foreach (array('id', 'type') as $key) {
    $form['entity_2'][$key] = array(
      '#type' => 'value',
      '#value' => $entity_2->$key->value,
    );
  }
  $form_state['form_display'] = entity_get_form_display($entity_1->entityType(), $entity_1->bundle(), 'default');
  field_attach_form($entity_2, $form['entity_2'], $form_state);

  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 100,
  );

  return $form;
}

/**
 * Validate handler for field_test_entity_nested_form().
 */
function field_test_entity_nested_form_validate($form, &$form_state) {
  $entity_1 = entity_create('entity_test', $form_state['values']);
  field_attach_extract_form_values($entity_1, $form, $form_state);
  field_attach_form_validate($entity_1, $form, $form_state);

  $entity_2 = entity_create('entity_test', $form_state['values']['entity_2']);
  field_attach_extract_form_values($entity_2, $form['entity_2'], $form_state);
  field_attach_form_validate($entity_2, $form['entity_2'], $form_state);
}

/**
 * Submit handler for field_test_entity_nested_form().
 */
function field_test_entity_nested_form_submit($form, &$form_state) {
  $entity_1 = entity_create('entity_test', $form_state['values']);
  field_attach_extract_form_values($entity_1, $form, $form_state);
  $entity_1->save();

  $entity_2 = entity_create('entity_test', $form_state['values']['entity_2']);
  field_attach_extract_form_values($entity_2, $form['entity_2'], $form_state);
  $entity_2->save();

  drupal_set_message(t('test_entities @id_1 and @id_2 have been updated.', array('@id_1' => $entity_1->id(), '@id_2' => $entity_2->id())));
}
