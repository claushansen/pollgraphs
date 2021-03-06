<?php

function pollgraphs_results_page($node){
	loadHeaderScript();
	$original_content = org_advpoll_results_page($node);
	$data = advpoll_get_data($node);
	$results = advpoll_get_votes($node->nid,$data->behavior);
    $final = advpoll_update_choices($data->choices, $results['choices']);
	$piechart = getChartScript($final, 'PieChart', 'poll results', '100%', '400px');
	$barchart = getChartScript($final, 'BarChart', 'poll results', '100%', '400px');
	$columnchart = getChartScript($final, 'ColumnChart', 'poll results', '100%', '400px');
	$content = array();
	
	$content['start_tabbable'] = array(
		'#type' => 'markup',
		'#markup' => '<div class="tabbable">',
	);
	
	$content['nav-tabs'] = array(
		'#type' => 'markup',
		'#markup' => 
		'<ul class="nav nav-pills">'.
		'<li class="active"><a href="#original" data-toggle="tab">Normal</a></li>'.
		'<li><a href="#piechart" data-toggle="tab">Google Piechart</a></li>'.
		'<li><a href="#barchart" data-toggle="tab">Google Barchart</a></li>'.
		'<li><a href="#columnchart" data-toggle="tab">Google Columnchart</a></li>'.
		'</ul>',
		'#prefix' => '<div class="col-md-12"; >',
		'#suffix' => '</div>'
	);
	
	$content['start_tabcontent'] = array(
		'#type' => 'markup',
		'#markup' => '<div class="tab-content col-md-12">',
	);
	
	$content['original_content'] = array(
		'#type' => 'markup',
		'#markup' => $original_content,
		'#prefix' => '<div id="original" class="col-md-12 tab-pane active"; >',
		'#suffix' => '</div>'
	);

	$content['piechart'] = array(
		'#type' => 'markup',
		'#markup' => $piechart,
		'#prefix' => '<div id="piechart" class="col-md-12 tab-pane">',
		'#suffix' => '</div>'
		);
	$content['barchart'] = array(
		'#type' => 'markup',
		'#markup' => $barchart,
		'#prefix' => '<div id="barchart" class="col-md-12 tab-pane">',
		'#suffix' => '</div>'
		);
		
	$content['columnchart'] = array(
		'#type' => 'markup',
		'#markup' => $columnchart,
		'#prefix' => '<div id="columnchart" class="col-md-12 tab-pane">',
		'#suffix' => '</div>'
		);
		
	$content['end_tabcontent'] = array(
		'#type' => 'markup',
		'#markup' => '</div>',
	);
		
	$content['end_tabbable'] = array(
		'#type' => 'markup',
		'#markup' => '</div>',
	);
	//adding script for updating charts when tab content opens
	//Needs to be added here as i could'nt get it placed after jQuery is loaded with drupal_add_js
	$content['script'] = array(
		'#type' => 'markup',
		'#markup' => 'jQuery(\'a[data-toggle="tab"]\').on(\'shown.bs.tab\', function (e) {
		console.log(\'test:\'+e.target);
		PieChartdrawChart();
		BarChartdrawChart();
		ColumnChartdrawChart();
		});',
		'#prefix' => '<script>',
		'#suffix' => '</script>'
	);
	return $content;
}

/* 
* Callback function for eksport csvfile
*
*/

function pollgraphs_export_csv($node){
	
	$filename = 'pollgraphs_export_node_'.$node->nid.'.csv';
	//setting headers
	$now = gmdate("D, d M Y H:i:s");
    header("Expires: Tue, 03 Jul 2001 06:00:00 GMT");
    header("Cache-Control: max-age=0, no-cache, must-revalidate, proxy-revalidate");
    header("Last-Modified: {$now} GMT");

    // force download  
    header("Content-Type: application/force-download");
    header("Content-Type: application/octet-stream");
    header("Content-Type: application/download");

    // disposition / encoding on response body
    header("Content-Disposition: attachment;filename={$filename}");
    header("Content-Transfer-Encoding: binary");
	
	$data = advpoll_get_data($node);
	$results = advpoll_get_votes($node->nid,$data->behavior);
    $final = advpoll_update_choices($data->choices, $results['choices']);
	
	//creating csvfile in memory
	ob_start();
	$csv = fopen("php://output", 'w');
   	fputcsv($csv, array_keys(reset($final)));
   	foreach ($final as $row) {
      fputcsv($csv, $row);
   	}
   	fclose($csv);
	echo ob_get_clean();
	die();

	
}

function org_advpoll_results_page($node) {
  drupal_add_css(drupal_get_path('module', 'advpoll') . '/css/advpoll.css', array('group' => CSS_DEFAULT, 'every_page' => TRUE));
  $data = advpoll_get_data($node);
  $output = '';

  if ($node) {
    $output = advpoll_display_results($node->nid, $data, 1);
  }

  return $output;
}

function loadHeaderScript(){
	drupal_add_js('https://www.google.com/jsapi', 'external');
	$script = 'google.load("visualization", "1", {packages:["corechart"]});'; 
	drupal_add_js($script,array('type' => 'inline', 'scope' => 'header', 'weight' => 5));
}