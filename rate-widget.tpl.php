<?php
// $Id$

/**
 * @file
 * Rate widget theme
 *
 * This is the default template for rate widgets. See section 3 of the README
 * file for information on theming widgets.
 */

foreach ($form as $name => $element) {
  if ($name{0} == '#') {
    continue; // Skip elements with a name starting with #
  }
  print drupal_render($element);
  if (isset($element['#votes'])) {
    print $element['#votes'];
  }
}

if (isset($form['#results']['rating'])) {
  print t('Rating') . ': ' . $form['#results']['rating'] . ' ';
}

print t('Vote count') . ': ' . $form['#results']['count'];

?>