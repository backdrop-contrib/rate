<?php
/**
 * @file
 * Rate widget theme
 *
 * This is the default template for rate widgets. See section 3 of the README
 * file for information on theming widgets.
 *
 */

// Calculate rating in number of stars.
$rating = round($results['rating'] / 25);

for ($i = 0; $i < 5; $i++) {
  if (round($results['rating'] / 25) >= $i) {
    $class = 'rate-fivestar-filled';
  }
  else {
    $class = 'rate-fivestar-empty';
  }
  print '<span class="' . $class . '">' . $links[$i]['content'] . '</span>';
}

?>