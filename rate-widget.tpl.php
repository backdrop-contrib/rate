<?php

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