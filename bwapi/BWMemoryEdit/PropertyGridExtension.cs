using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace BWMemoryEdit
{
    public static class PropertyGridExtension
    {
        // ******************************************************************
        public static void ExpandGroupName(this PropertyGrid propertyGrid, string groupName)
        {
            foreach (GridItem gridItem in propertyGrid.SelectedGridItem.GridItems)
            {
                if (gridItem != null)
                {
                    if (gridItem.GridItemType == GridItemType.Category && gridItem.Label == groupName)
                    {
                        gridItem.Expanded = true;
                    }
                }
            }
        }

        // ******************************************************************
        public static void ExpandItemWithInitialExpandedAttribute(this PropertyGrid propertyGrid)
        {
            ExpandItemWithInitialExpandedAttribute(propertyGrid, propertyGrid.SelectedGridItem);
        }

        // ******************************************************************
        private static void ExpandItemWithInitialExpandedAttribute(PropertyGrid propertyGrid, GridItem gridItem)
        {
            if (gridItem != null)
            {
                if (gridItem.GridItemType == GridItemType.Property && gridItem.Expandable)
                {
                    object[] objs = gridItem.Value.GetType().GetCustomAttributes(typeof(PropertyGridInitialExpandedAttribute), false);
                    if (objs.Length > 0)
                    {
                        if (((PropertyGridInitialExpandedAttribute)objs[0]).InitialExpanded)
                        {
                            gridItem.Expanded = true;
                        }
                    }
                }

                foreach (GridItem childItem in gridItem.GridItems)
                {
                    ExpandItemWithInitialExpandedAttribute(propertyGrid, childItem);
                }
            }
        }

        // ******************************************************************
    }
}
