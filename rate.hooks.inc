<?php

/**
 * Define templates for rate widgets.
 *
 * @return array
 */
function hook_rate_templates() {
  $templates = array();

  $templates['thumbs_up'] = new stdClass();
  $templates['thumbs_up']->value_type = 'points';
  $templates['thumbs_up']->options = array(
    array(1, 'up'),
  );

  $templates['thumbs_up_down'] = new stdClass();
  $templates['thumbs_up_down']->value_type = 'points';
  $templates['thumbs_up_down']->options = array(
    array(1, 'up'),
    array(-1, 'down'),
  );

  $templates['fivestar'] = new stdClass();
  $templates['fivestar']->value_type = 'percent';
  $templates['fivestar']->options = array(
    array(0, '*'),
    array(25, '**'),
    array(50, '***'),
    array(75, '****'),
    array(100, '*****'),
  );

  return $templates;
}