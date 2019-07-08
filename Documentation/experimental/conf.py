import sys
import sphinx_bootstrap_theme

sys.path.append("./breathe/")

def linkcode_resolve(domain, info):
    if domain != 'cpp':
        return None
    if not info['names']:
        return None
    filename = '/'.join(info['names'])
    return "https://github.com/bwapi/bwapi/blob/master/bwapi/include/%s" % filename

#####################################################################

extensions = [
#  'sphinx.ext.autosectionlabel',
#  'sphinx.ext.autosummary',
  'sphinx.ext.extlinks',
  'sphinx.ext.graphviz',
  'sphinx.ext.inheritance_diagram',
  'sphinx.ext.linkcode',
  'sphinx.ext.todo',
  #'sphinxcontrib.issuetracker',
  #'sphinxcontrib.programoutput',
  #'sphinxcontrib.twitter',
  #'sphinxcontrib.youtube',
  #'sphinxcontrib.googleanalytics',
  #'sphinxcontrib.recentpages',
  #'tut.sphinx', # For i.e. BWAPI4 to 5 changes
  'm2r',  # markdown to rst
  'breathe',
  'exhale'
]

source_suffix = ['.rst', '.md']

breathe_projects = { "BWAPI": "./doxyout/xml" }
breathe_default_project = "BWAPI"
breathe_default_members = ('members', 'undoc-members')
breathe_show_define_initializer = True
breathe_implementation_filename_extensions = ['.cpp', '.cxx', '.dox']

# Setup the exhale extension
exhale_args = {
    # These arguments are required
    "containmentFolder":     "./api",
    "rootFileName":          "library_root.rst",
    "rootFileTitle":         "Library API",
    "doxygenStripFromPath":  "..",
    # Suggested optional arguments
    "createTreeView":        True,
    # TIP: if using the sphinx-bootstrap-theme, you need
    #"treeViewIsBootstrap": True,
    #"exhaleExecutesDoxygen": True,
    #"exhaleUseDoxyfile": True,
    
    "minifyTreeView": True,
    "fullToctreeMaxDepth": 1,
    "listingExclude": [r"std::.*"],
    #"unabridgedOrphanKinds": ['define', 'dir', 'file'],
    #"kindsWithContentsDirectives": ['file', 'namespace'],
}

# Tell sphinx what the primary language being documented is.
primary_domain = 'cpp'

# Tell sphinx what the pygments highlight language should be.
highlight_language = 'cpp'

language = "en"
project = "BWAPI"
author = "BWAPI Team"
version = "5.0.0"
release = "5.0.0 Alpha"

html_show_copyright = False
html_show_sphinx = False
html_show_sourcelink = True
html_experimental_html5_writer = True
html_theme = 'sphinx_rtd_theme'
html_theme_options = {
  #'style_external_links': True,
}

todo_include_todos = True

