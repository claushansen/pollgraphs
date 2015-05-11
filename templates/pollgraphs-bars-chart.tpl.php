<?php

/**
 * @file
 * Template for Pollgraphs Bar Charts
 *
 * Variables available:
 * - $nid: The nid of the poll
 * - $cancel_form: Provides a form for deleting user's votes when they have 
 *   permission to do so.
 * - $bars: The output of all styled bars displaying votes.
 * - $total: Total number of votes.
 * - $voted: An array indicating which unique choice ids were selected by the user.
 *
 */
// print $chart;
?>
<div class="poll" id="advpoll-<?php print $nid; ?>">
<div class="col-md-12">
<?php print $chart; ?>
</div>    
    <div class="total"><?php print t('Total votes: @total', array('@total' => $total)); ?></div>

    <?php if ($voted): ?>
    <div class="poll-message"><?php print t('Thank you for voting.'); ?></div>
    <?php endif; ?>
    
    <?php print $cancel_form; ?>
    
</div>
