<?php

/**
 * @file
 * Rate widget theme
 */

print theme('item_list', array('items' => $stars));

if ($info) {
  print '<div class="rate-info">' . $info . '</div>';
}

?>