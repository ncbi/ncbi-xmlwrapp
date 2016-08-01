<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">

<xsl:template match="/">
    <p><xsl:value-of select="world/number" /></p>
    <p><xsl:value-of select="world/number / world/value" /></p>
    <p><xsl:value-of name="number" select="world/number" /></p>
    <p><xsl:value-of name="number" select="$number" /></p>
</xsl:template>

</xsl:stylesheet>
