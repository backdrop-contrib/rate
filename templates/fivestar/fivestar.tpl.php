<?php
/**
 * @file
 * Rate widget theme
 */

print theme('item_list', $stars);

if ($info) {
  print '<div class="rate-info">' . $info . '</div>';
}

?>