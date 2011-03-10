<?php
/**
 * @file
 * Rate widget theme
 */

print theme('item_list', array('items' => $buttons));

if ($info) {
  print '<div class="rate-info">' . $info . '</div>';
}

?>