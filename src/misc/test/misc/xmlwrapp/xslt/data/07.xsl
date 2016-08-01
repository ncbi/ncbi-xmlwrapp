<?xml version="1.0" encoding="ISO-8859-1"?>
<!-- Edited by XMLSpy® -->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:template match="build_outputs">
    <table border="1" width="800" cellspacing="1" cellpadding="1" >
      <tr bgcolor="#66CCFF">
        <th>Build Run</th>
        <th>Task type</th>
        <th>Plane</th>
        <th>Task Name</th>
        <th>Role</th>
        <th>Path</th>
      </tr>
      <xsl:for-each select="build_output">
      <tr>
        <td><xsl:value-of select="build_run"/></td>
        <td><xsl:value-of select="task_type"/></td>
        <td><xsl:value-of select="plane"/></td>
        <td><xsl:value-of select="task_name"/></td>
        <td><xsl:value-of select="role"/></td>
        <td>
          <xsl:choose>
          <xsl:when test="count(out_file)>0">
            <a href="file:/{out_file/path}" target="_blank"> 
              <xsl:value-of select="out_file/name"/>
            </a>
          </xsl:when>
          <xsl:otherwise>No Output.</xsl:otherwise>
          </xsl:choose>
        </td>
      </tr>
      </xsl:for-each>
    </table>
</xsl:template>
</xsl:stylesheet>


