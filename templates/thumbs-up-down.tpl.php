<?php
/**
 * @file
 * Rate widget theme
 */
?>
<div class="rate-thumbs-up-down-rating-up"><?php print $results['up_percent'] . '%'; ?></div>
<?php
print $up_button;
print $down_button;
?>
<div class="rate-thumbs-up-down-rating-down"><?php print $results['down_percent'] . '%'; ?></div>
<?php

if ($info) {
  print '<div class="rate-info">' . $info . '</div>';
}

?>