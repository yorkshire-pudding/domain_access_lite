<?php

/**
 * @file
 * Admin page callbacks for the domain_access_lite module.
 */

function domain_access_lite_admin(&$form_state, $edit = array()) {
  $edit += array(
    'nodes' => array(),
  );

  $form = array();

  $domain_access_lite_vid = variable_get('domain_access_lite_vid', 0);
  if ($domain_access_lite_vid) {
    $vocabulary = taxonomy_vocabulary_load($domain_access_lite_vid);

    $form['domain_access_lite_vid_text'] = array(
      '#prefix' => '<p>',
      '#summary' => t('The taxonomy vocabulary <a href="!edit-url">%name</a> is set as the default for domain access.', array('!edit-url' => url('admin/content/taxonomy/'. $domain_access_lite_vid), '%name' => $vocabulary->name)),
      '#suffix' => '</p>',
    );
    $form['domain_access_lite_vid_text']['#value'] = $form['domain_access_lite_vid_text']['#summary'];
  }

  $vocabs = array();
  $vocabularies = taxonomy_get_vocabularies();
  foreach ($vocabularies as $vid => $vocabulary) {
    $vocabs[$vid] = $vocabulary->name;
  }

  $form['domain_access_lite_vid'] = array(
    '#type' => 'select',
    '#title' => t('Doamin Access vocabulary'),
    '#description' => t("The taxonomy vocabulary that will be considered the default for domain access."),
    '#default_value' => variable_get('domain_access_lite_vid', 0),
    '#summary callback' => 'summarize_null',
    '#options' => $vocabs,
  );

  if ($domain_access_lite_vid) {
    $form['domains'] = array(
      '#type' => 'fieldset',
      '#title' => t('Domain mappings'),
    );
    $result = db_query(db_rewrite_sql('SELECT t.tid, t.* FROM {term_data} t WHERE t.vid = %d ORDER BY weight', 't', 'tid'), $domain_access_lite_vid);
    while ($term = db_fetch_object($result)) {
      $form['domains']['domain_access_lite_domain_' . strtolower($term->name)] = array(
        '#type' => 'textfield',
        '#title' => t('!domain mapping', array('!domain' => $term->name)),
        '#default_value' => variable_get('domain_access_lite_domain_' . strtolower($term->name), ''),
      );
    }
  }

  return system_settings_form($form);
}

function domain_access_lite_admin_validate($form, &$form_state) {
  if (!form_get_errors()) {
    if ($form_state['values']['domain_access_lite_vid'] != variable_get('domain_access_lite_vid', 0)) {
      // Notify that node access needs to be rebuilt.
      node_access_needs_rebuild(TRUE);
    }
    $result = db_query(db_rewrite_sql('SELECT t.tid, t.* FROM {term_data} t WHERE t.vid = %d ORDER BY weight', 't', 'tid'), $domain_access_lite_vid);
    while ($term = db_fetch_object($result)) {
      $key = 'domain_access_lite_domain_' . strtolower($term->name);
      if ($form_state['values'][$key] != variable_get($key, '')) {
        // Notify that node access needs to be rebuilt.
        node_access_needs_rebuild(TRUE);
      }
    }
  }
}
