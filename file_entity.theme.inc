<?php

/**
 * @file
 * Theme callbacks for the file entity module.
 */

/**
 * A copy of theme_file_file_link() that makes the link point to file/[fid].
 *
 * @see theme_file_file_link()
 * @return type
 */
function theme_file_entity_file_link($variables) {
  $file = $variables['file'];
  $icon_directory = $variables['icon_directory'];

  $url = 'file/' . $file->fid;
  $icon = theme('file_icon', array('file' => $file, 'icon_directory' => $icon_directory));

  // Set options as per anchor format described at
  // http://microformats.org/wiki/file-format-examples
  $options = array(
    'attributes' => array(
      'type' => $file->filemime . '; length=' . $file->filesize,
    ),
  );

  // Use the description as the link text if available.
  if (empty($file->description)) {
    $link_text = $file->filename;
  }
  else {
    $link_text = $file->description;
    $options['attributes']['title'] = check_plain($file->filename);
  }

  return '<span class="file">' . $icon . ' ' . l($link_text, $url, $options) . '</span>';
}

/**
 * Own implementation of the image_style theme to bypass the browser cache.
 */
function theme_file_entity_image_style($variables) {
  // Determine the dimensions of the styled image.
  $dimensions = array(
    'width' => $variables['width'],
    'height' => $variables['height'],
  );

  image_style_transform_dimensions($variables['style_name'], $dimensions);

  $variables['width'] = $dimensions['width'];
  $variables['height'] = $dimensions['height'];

  // Determine the URL for the styled image.
  $variables['path'] = image_style_url($variables['style_name'], $variables['path']);
  // Add the file modification time to the path to bypass the browser cache.
  $path = drupal_parse_url($variables['path']);
  $internal_path = str_replace($GLOBALS['base_url'] . '/', '', $path['path']);
  if (is_file($internal_path)) {
    $mtime = filemtime($internal_path);
  }
  else {
    $mtime = 0;
  }
  $variables['path'] .= (strpos($variables['path'], '?') !== FALSE) ? '&' : '?';
  $variables['path'] .= 't=' . $mtime;
  return theme('image', $variables);
}
