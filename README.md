This is a 3rd release (v2.3.0) of NCBI branch of library `XmlWrapp`.

It was originally branched off the main `xmlwrapp` project release 0.6.0 on
March 11, 2009. Since then we extended it with numerous new features:

* XML namespace support
* XPath support
* Extended error handling
* XSLT extension functions
** HTTPS support (sorry, not in this release because of packaging issues)
* Elements support
* Canonicalization of the documents
* ...and more, including numerous bugfixes.

Some of the NCBI changes were submitted and reintegrated into the main `xmlwrapp`
branch, and conversely the NCBI's version of the library was patched with some
of the changes made in the main XmlWrapp branch.

The sources have been rearranged to fit the NCBI C++ Toolkit build framework.
Also, due to the sheer number of major changes, extensions and reductions (and
also due to some irreconcilable differences with the main branch in e.g.
implementation of the namespace support) the NCBI fork is currently
incompatible with the `xmlwrapp-0.6.0` and its other successors.

Here you can see the full list of changes (sans demo and test code) that went
into the 2.3.0 release:

* [https://www.ncbi.nlm.nih.gov/viewvc/v1/trunk/c%2B%2B/include/misc/xmlwrapp/?view=log&pathrev=70748]
* [https://www.ncbi.nlm.nih.gov/viewvc/v1/trunk/c%2B%2B/src/misc/xmlwrapp/?view=log&pathrev=70748]

