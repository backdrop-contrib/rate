<?php

/**
 * @file
 * Rate widget theme
 *
 * This is the default template for rate widgets. See section 3 of the README
 * file for information on theming widgets.
 */

print theme('item_list', $buttons);

if ($info) {
  print '<div class="rate-info">' . $info . '</div>';
}

?>

