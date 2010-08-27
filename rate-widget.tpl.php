<?php
// $Id$

/**
 * @file
 * Rate widget theme
 *
 * This is the default template for rate widgets. See section 3 of the README
 * file for information on theming widgets.
 */

foreach ($links as $link) {
  print theme('rate_button', $link['text'], $link['href']);
}

?>