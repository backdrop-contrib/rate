<?php
/**
 * @file
 * Rate widget theme
 */

print '<div class="rate-slider rate-value-' . $value . '">';
print theme('item_list', $buttons);
print '</div>';

if ($info) {
  print '<div class="rate-info">' . $info . '</div>';
}

?>