<?php

/**
 * Clean up comment view link field. Give site builders the option of outputting
 * the full URL instead of an anchor with a 'view' title and an absolute link.
 */
class simple_blog_handler_field_comment_link extends views_handler_field_comment_link {
  function render($values) {
    $text = !empty($this->options['text']) ? $this->options['text'] : t('view');
    return url("node/" . $values->{$this->aliases['nid']}, array('absolute' => TRUE, 'html' => TRUE, 'fragment' => "comment-" . $values->{$this->aliases['cid']}));
    return l($text, "node/" . $values->{$this->aliases['nid']}, array('html' => TRUE, 'fragment' => "comment-" . $values->{$this->aliases['cid']}));
  }
}