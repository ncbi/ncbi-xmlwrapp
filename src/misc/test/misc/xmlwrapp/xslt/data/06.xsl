<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output method="text" indent="no" encoding="us-ascii"/>
<xsl:template match="/"><xsl:value-of select="root/foo[1]/bar[3]"/></xsl:template>
</xsl:stylesheet>
