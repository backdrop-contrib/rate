<?php
/**
 * @file
 * Rate widget theme
 */
?>
<ul>
  <li class="thumb-up">
    <?php print $up_button; ?>
    <?php print $results['up_percent'] . '%'; ?>
  </li>
  <li class="thumb-down">
    <?php print $down_button; ?>
    <?php print $results['down_percent'] . '%'; ?>
  </li>
</ul>
<?php
if ($info) {
  print '<div class="rate-info">' . $info . '</div>';
}

?>

