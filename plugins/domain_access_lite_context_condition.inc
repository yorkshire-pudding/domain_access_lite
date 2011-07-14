<?php

/**
 * @file
 * Domain context condition.
 */

/**
 * Expose current domain as a context condition
 */
class domain_access_lite_domain_context_condition extends context_condition {
  function condition_values() {
    $domains = domain_access_lite_get_domains();

    $conditions = array();
    foreach ($domains as $term) {
      $conditions[$term->tid] = $term->name;
    }

    return $conditions;
  }

  /**
   * Execute.
   */
  function execute($value) {
    foreach ($this->get_contexts($value) as $context) {
      $this->condition_met($context);
    }
  }
}
