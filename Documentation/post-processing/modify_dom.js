var system = require('system');
var fs = require('fs');
var page = require('webpage').create();

function modifyPage(filename) {
  console.log("Modifying " + filename);
  
  var rawPageString = fs.read(filename);
  rawPageString = rawPageString.replace(/<script type="text\/javascript"/g, "<script type='foo/bar'");
  rawPageString = rawPageString.replace(/<script>/g, "<script type='foo/bar'>");

  page.content = rawPageString;

  page.injectJs("jquery-2.1.4.min.js");
  page.evaluate(function () {
    // Remove obsolete attributes
    $("img[border]").removeAttr("border");
    $("table[border]").removeAttr("border");
    $("iframe[frameborder]").removeAttr("frameborder");
    $(".memItemLeft[align]").removeAttr("align");
    $(".memItemLeft[valign]").removeAttr("valign");
    $(".memItemRight[valign]").removeAttr("valign");
    $("script[type]").removeAttr("type");
    
    // Remove pointless "even" class (replaced with CSS nth-child selector)
    $(".even").removeClass("even");

    // Remove invalid attributes
    $("a[doxygen]").removeAttr("doxygen");
    
    // Remove hidden elements (that we don't want to show anymore)
    $("tr[class^=separator\\:]").remove();
    $(".memSeparator").remove();
    
    // Change <a name=""> to <a id="">
    $("a[name]").each(function() {
      var elem = $(this);
      elem.attr("id", elem.attr("name"));
      elem.removeAttr("name");
    });
    
    // Reduce complexity of prototype displays
    $("div.memproto").each(function(){
      var result = "";
      var found = false;   // Purpose of this is to prevent corruption on accidental multiple runs
      $(this).find("table.memname > tbody > tr > td").each(function(){
        found = true;
        if ($(this).hasClass("paramname")) {
          var paramText = $(this).html();
          if (paramText) {
            result += $("<span>").addClass("paramname").html(paramText).prop("outerHTML");
          }
        } else {
          result += $(this).html();
        }
      });
      var span = $(this).find("td.mlabels-right");
      if (span.length > 0) {
        result += span.html();
      }
      if (found) {
        $(this).html(result);
      }
    });
    
    // Reduce cruft in table of contents
    $("tr[class^=memitem\\:]").removeAttr("class");
    
    // Cleanup
    $("script[type^=foo]").removeAttr("type");
  });

  fs.write(filename, page.content, 'w');
}

function traverse(path) {
  if (fs.isFile(path) && /\.html$/.test(path)) {
    modifyPage(path);
  } else if (fs.isDirectory(path) && !fs.isLink(path)) {
    var files = fs.list(path);
    for (var i = 0; i < files.length; ++i) {
      if (files[i] != "." && files[i] != "..") {
        traverse(path + "/" + files[i]);
      }
    }
  }
}

traverse(system.args[1]);
phantom.exit();
