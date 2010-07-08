$Id$

PREFACE
-------
This module provides flexible voting widgets for nodes and comments.
Administrators can add multiple widgets and define an unlimited number of
buttons.

CONTENTS
--------
1. Installation
2. Configuration
2.1 Widget configuration
3. Theming

1. Installation
--------------------------------------------------------------------------------
Before installing Rate, you need VotingAPI. If not already installed, download
the latest stable version at http://drupal.org/project/votingapi
Please follow the readme file provided by VotingAPI on how to install.

Copy Rate into your modules directory (i.e. sites/all/modules) and enable Rate
(on admin/build/modules).

2. Configuration
--------------------------------------------------------------------------------
After installation, the configuration page will be available at
admin/build/rate. This page shows a list with available widgets. Each widget
have an edit and delete link. You can add a tab on the tab 'Add widget'. Both
editing and adding a widget leads to the widget configuration (see section 2.1).

2.1. Widget configuration
--------------------------------------------------------------------------------
The widget configuration form has the following elements:

* Title
  The title is only used in the admin section. Use a recognizable name.
* Machine readable name
  Name used for technical purposes. You may only contain alphanumeric characters
  and underscores.
* Value type
  This determines how vote results are totaled. VotingAPI supports three value
  types by default: 'Percentage' votes are averaged, 'Points' votes are summed
  up, and 'Options' votes get a count of votes cast for each specific option.
  Typical usages are:
  * Thumbs up / down: use points
  * Bad / mediocre / good: use percentage
  * Makes me funny / boring / mad / angry: use options
* Options
  These are the options displayed to the user. Each option has a value and a
  label.
* Node types
  Check the node types on which a rate widget should be available. There are
  separate columns for nodes and comments in this table.

3. Theming
--------------------------------------------------------------------------------
Available templates for theming are:

* rate-widget.tpl.php
  This is the default template for all widgets.
* rate-widget--NAME.tpl.php
  This is a widget specific template. Use the machine name for NAME. Replace
  underscores by dashes in this name.

You may use the following snippets in the template:

* Print a button for a single option:

    <?php print drupal_render($form[$form['#option_id'][1]]); ?>

  '1' is the value for the option (see section 2.1). Thus for a thumbs up / down
  configuration you will have:

    <?php print drupal_render($form[$form['#option_id'][1]]); ?>
    <?php print drupal_render($form[$form['#option_id'][-1]]); ?>

* Print the rating when using value type 'percentage' or 'points':

    <?php print $form['#results']['rating']; ?>

* Print the number of votes for a specific option (only available when using
  value type 'options'):

    <?php print $form[$form['#option_id'][1]]['#votes']; ?>

  '1' is the value for the option (see section 2.1).

* Print the total number of votes:

  <?php print $form['#results']['count']; ?>