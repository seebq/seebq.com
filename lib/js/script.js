jQuery( function($) {
	$('.buttons a').each( function() {
		$(this).attr('sbtooltip', $(this).attr("title") ).removeAttr("title");
	});
});