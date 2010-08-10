<?php
// $Id$

/**
 * @file
 * Rating admin
 */

/**
 * Menu callback.
 *
 * Generates the admin page which contains the widget listing.
 */
function rate_admin_page() {
  $widgets = variable_get(RATE_VAR_WIDGETS, array());
  uasort($widgets, '_rate_sort');

  $header = array(t('Title'), t('Name'), t('Operations'));
  $rows = array();
  foreach ($widgets as $id => $widget) {
    $edit = l(t('Edit'), str_replace('%', $id, RATE_PATH_ADMIN_PAGE_EDIT));
    $delete = l(t('Delete'), str_replace('%', $id, RATE_PATH_ADMIN_PAGE_DELETE));
    $rows[] = array(
      $widget->title,
      $widget->name,
      "$edit $delete",
    );
  }

  return theme('table', $header, $rows);
}

/**
 * Callback for uasort().
 *
 * @see rate_admin_page()
 * @see http://php.net/uasort
 */
function _rate_sort($a, $b) {
  if ($a->title == $b->title) {
    return 0;
  }
  return $a->title < $b->title ? -1 : 1;
}

/**
 * Form for adding and editing widgets.
 */
function rate_widget_form(&$form_state, $id = NULL) {
  $form = array();

  if ($id) {
    $form['#widget_id'] = $id;
  }

  if ($id) {
    $widgets = variable_get(RATE_VAR_WIDGETS, array());
    if (!isset($widgets[$id])) {
      drupal_not_found();
      module_invoke_all('exit') & exit;
    }
    $widget = $widgets[$id];
  } else {
    $widget = new stdClass();
    $widget->name = '';
    $widget->tag = 'vote';
    $widget->title = '';
    $widget->node_types = array();
    $widget->comment_types = array();
    $widget->value_type = 'percent';
    $widget->options = array();
  }

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $widget->title,
    '#required' => TRUE,
  );

  /**
   * @todo Implement http://drupal.org/node/302054#comment-1110505
   */
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Machine readable name'),
    '#default_value' => $widget->name,
    '#required' => TRUE,
  );

  $form['tag'] = array(
    '#type' => 'textfield',
    '#title' => t('Tag'),
    '#default_value' => $widget->tag,
    '#description' => t('Tag used for VotingAPI. Widgets with the same tag share their voting results.'),
    '#required' => TRUE,
  );

  $options = array(
    'percent' => t('Percentage'),
    'points' => t('Points'),
    'option' => t('Options'),
  );
  $form['value_type'] = array(
    '#type' => 'radios',
    '#title' => t('Value type'),
    '#options' => $options,
    '#default_value' => $widget->value_type,
    '#required' => TRUE,
  );

  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Options'),
    '#description' => t('Define the available voting buttons.'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  $form['options']['options'] = array(
    '#theme' => 'rate_admin_options',
    '#prefix' => "<div id=\"rate-options\">",
    '#suffix' => '</div>',
  );

  if ($form_state['submitted']) {
    $option_count = (int) $form_state['values']['option_count'];
    if ($form_state['clicked_button']['#id'] == 'edit-add-another') {
      ++$option_count;
    }
  } else {
    $option_count = max(2, count($widget->options));
  }

  $id = 0;
  $c = $option_count;
  for ($i = 0; $i < $c; $i++) {
    if ($form_state['submitted'] && !empty($form_state['values']['delete' . $i])) {
      --$option_count;
      continue;
    }
    $form['options']['options']['option' . $id] = array();
    if (isset($form_state['values']['value' . $i])) {
      $default_value = $form_state['values']['value' . $i];
      $default_label = $form_state['values']['label' . $i];
    }
    elseif (isset($widget->options[$i]) && !$form_state['submitted']) {
      $default_value = $widget->options[$i][0];
      $default_label = $widget->options[$i][1];
    }
    else {
      $default_value = '';
      $default_label = '';
    }
    $form['options']['options']['option' . $id]['value' . $id] = array(
      '#type' => 'textfield',
      '#title' => t('Value'),
      '#default_value' => $default_value,
      '#size' => 16,
    );
    $form['options']['options']['option' . $id]['label' . $id] = array(
      '#type' => 'textfield',
      '#title' => t('Label'),
      '#default_value' => $default_label,
      '#size' => 40,
    );
    $form['options']['options']['option' . $id]['delete' . $id] = array(
      '#type' => 'checkbox',
      '#title' => t('Delete'),
      '#default_value' => FALSE,
    );
    ++$id;
  }

  $form['options']['option_count'] = array(
    '#type' => 'hidden',
    '#value' => $option_count,
  );

  $form['options']['add_another'] = array(
    '#type' => 'submit',
    '#value' => t('Add another option'),
    '#ahah' => array(
      'path' => RATE_PATH_ADMIN_AHAH,
      'wrapper' => 'rate-options',
      'method' => 'replace',
      'effect' => 'none',
    ),
  );

  $form['types'] = array(
    '#type' => 'fieldset',
    '#title' => t('Node types'),
    '#description' => t('Select the node types on which to enable this widget.'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#theme' => 'rate_admin_types',
  );
  $sql = 'SELECT type, name FROM {node_type} ORDER BY name ASC';
  $res = db_query($sql);
  while ($rec = db_fetch_array($res)) {
    $form['types'][$rec['type']] = array(
      '#type' => 'fieldset',
      '#title' => $rec['name'],
    );
    $form['types'][$rec['type']]['node_type_' . $rec['type']] = array(
      '#type' => 'checkbox',
      '#title' => $rec['name'],
      '#default_value' => in_array($rec['type'], $widget->node_types),
    );
    $form['types'][$rec['type']]['comment_type_' . $rec['type']] = array(
      '#type' => 'checkbox',
      '#title' => $rec['name'],
      '#default_value' => in_array($rec['type'], $widget->comment_types),
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  
  return $form;
}

/**
 * Form validate.
 */
function rate_widget_form_validate($form, &$form_state) {
  // Validate machine readable name
  if (!preg_match('/^[a-z\\_0-9]+$/i', $form_state['values']['name'])) {
    form_set_error('name', t('The machine readable name may only contain alphanumeric characters and underscores.'));
  }

  // Validate vote tag
  if (!preg_match('/^[a-z\\_0-9]+$/i', $form_state['values']['tag'])) {
    form_set_error('tag', t('The vote tag may only contain alphanumeric characters and underscores.'));
  }

  // Validate option values
  $used_values = array();
  foreach ($form_state['values'] as $name => $value) {
    if (preg_match('/^value([0-9]+)$/', $name, $match)) {
      if (empty($value)) {
        continue;
      }
      if (!preg_match('/^\\-?[0-9]+$/', $value)) {
        form_set_error($name, t('Values must be integers'));
        continue;
      }
      if (in_array($value, $used_values)) {
        form_set_error($name, t('You may not use the same value twice.'));
      }
      $used_values[] = $value;
      if ($form_state['values']['value_type'] == 'percent') {
        if ($value < 0 || $value > 100) {
          form_set_error($name, t('Percentages must be between 0 and 100'));
        }
      }
    }
  }
  if (!count($used_values)) {
    form_set_error('', t('Each widget must have at least one options.'));
  }
}

/**
 * Form submit handler.
 */
function rate_widget_form_submit($form, &$form_state) {
  if ($form_state['clicked_button']['#id'] == 'edit-add-another') {
    $form_state['rebuild'] = TRUE;
    return $form_state['values'];
  }
  
  $widget = new stdClass();
  $widget->name = $form_state['values']['name'];
  $widget->tag = $form_state['values']['tag'];
  $widget->title = $form_state['values']['title'];
  $widget->node_types = array();
  $widget->comment_types = array();
  $widget->value_type = $form_state['values']['value_type'];
  $widget->options = array();

  foreach ($form_state['values'] as $name => $value) {
    if (preg_match('/^value([0-9]+)$/', $name, $match) && !empty($value)) {
      $widget->options[] = array(
        $value,
        $form_state['values']['label' . $match[1]],
      );
    }
    if (preg_match('/^node_type_(.+)$/', $name, $match) && $value) {
      $widget->node_types[] = $match[1];
    }
    if (preg_match('/^comment_type_(.+)$/', $name, $match) && $value) {
      $widget->comment_types[] = $match[1];
    }
  }

  $widgets = variable_get(RATE_VAR_WIDGETS, array());
  if (!empty($form['#widget_id'])) {
    $id = (int) $form['#widget_id'];
  }
  else {
    $id = 1;
    while (isset($widgets[$id])) {
      ++$id;
    }
  }
  $widgets[$id] = $widget;
  variable_set(RATE_VAR_WIDGETS, $widgets);
  drupal_set_message(t('The widget has been saved.'));

  $form_state['redirect'] = RATE_PATH_ADMIN_PAGE;
}

/**
 * AHAH callback for widget form.
 */
function rate_widget_form_ahah() {
  $form_state = array('storage' => NULL, 'submitted' => FALSE);
  $form_build_id = $_POST['form_build_id'];
  $form = form_get_cache($form_build_id, $form_state);
  $args = $form['#parameters'];
  $form_id = array_shift($args);
  $form['#post'] = $_POST;
  $form['#redirect'] = FALSE;
  $form['#programmed'] = FALSE;
  $form_state['post'] = $_POST;
  drupal_process_form($form_id, $form, $form_state);
  $form = drupal_rebuild_form($form_id, $form_state, $args, $form_build_id);
  $container = $form['options']['options'];
  unset($container['#prefix'], $container['#suffix']); // Prevent duplicate wrappers.
  $javascript = drupal_add_js(NULL, NULL, 'header');
  drupal_json(array(
    'status' => TRUE,
    'data' => drupal_render($container),
    'settings' => call_user_func_array('array_merge_recursive', $javascript['setting']),
  ));
}

/**
 * Theming function for the node / comment types list in the widget form.
 *
 * @ingroup themeable
 */
function theme_rate_admin_types($element) {
  $header = array(
    t('Name'),
    t('Node'),
    t('Comment'),
  );
  $rows = array();
  foreach ($element as $name => $subelement) {
    if ($name[0] != '#') {
      unset($subelement['node_type_' . $name]['#title']);
      unset($subelement['comment_type_' . $name]['#title']);
      $rows[] = array(
        check_plain($subelement['#title']),
        drupal_render($subelement['node_type_' . $name]),
        drupal_render($subelement['comment_type_' . $name]),
      );
    }
  }
  return theme('table', $header, $rows);
}

/**
 * Theme the options list in the widget form.
 *
 * @ingroup themeable
 */
function theme_rate_admin_options($element) {
  $header = array(
    t('Value'),
    t('Label'),
    t('Delete'),
  );
  $rows = array();
  foreach ($element as $name => $subelement) {
    if (preg_match('/^option([0-9]+)$/', $name, $match)) {
      $id = $match[1];
      unset($subelement['value' . $id]['#title']);
      unset($subelement['label' . $id]['#title']);
      unset($subelement['delete' . $id]['#title']);
      $rows[] = array(
        drupal_render($subelement['value' . $id]),
        drupal_render($subelement['label' . $id]),
        drupal_render($subelement['delete' . $id]),
      );
    }
  }
  return theme('table', $header, $rows);
}

/**
 * Form for deleting widgets.
 */
function rate_widget_delete_form(&$form_state, $id) {
  $form = array();

  $widgets = variable_get(RATE_VAR_WIDGETS, array());

  $form['#widget_id'] = $id;

  if (!$title = $widgets[$id]->title) {
    drupal_not_found();
    module_invoke_all('exit') & exit;
  }
  
  $form['info'] = array(
    '#value' => '<p>' . t('Are you sure you want to delete widget %title?', array('%title' => $title)) . '</p>'
  );

  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
  );

  $form['buttons']['cancel'] = array(
    '#value' => l('Cancel', RATE_PATH_ADMIN_PAGE),
  );

  return $form;
}

/**
 * Submit handler.
 */
function rate_widget_delete_form_submit($form, &$form_state) {
  $widgets = variable_get(RATE_VAR_WIDGETS, array());
  if ($title = $widgets[$form['#widget_id']]->title) {
    unset($widgets[$form['#widget_id']]);
    variable_set(RATE_VAR_WIDGETS, $widgets);
    drupal_set_message(t('Widget %title has been deleted.', array('%title' => $title)));
  }
  $form_state['redirect'] = RATE_PATH_ADMIN_PAGE;
}