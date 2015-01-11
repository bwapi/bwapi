using Binarysharp.MemoryManagement;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace BWMemoryEdit
{
    public partial class MainWindow : Form
    {
        private Dictionary<object, Dictionary<object, String>> listItemNameCache = new Dictionary<object, Dictionary<object, String>>();

        public MainWindow()
        {
            InitializeComponent();

            Process[] processes = Process.GetProcessesByName("StarCraft");
            if (processes.Length > 0)
            {
                MemorySharp sharp = new MemorySharp(processes[0]);
                // Init units
                for (int i = 0; i < 1700; ++i)
                {
                    IntPtr unitMemory = new IntPtr(0x0059CCA8 + i * 336);
                    Unit unit = new Unit(sharp[sharp.MakeRelative(unitMemory)], i);
                    unitList.Items.Add(unit);
                }
                // Init sprites
                for (int i = 0; i < 2500; ++i)
                {
                    IntPtr spriteMem = new IntPtr(0x00629D98 + i * 36);
                    Sprite sprite = new Sprite(sharp[sharp.MakeRelative(spriteMem)], i);
                    spriteList.Items.Add(sprite);
                }
            }
        }

        private void offsetList_SelectedIndexChanged(object sender, EventArgs e)
        {
            ListBox list = sender as ListBox;
            if (list != null && offsetTabs.SelectedTab.Contains(list))
            {
                editorGrid.SelectedObject = list.SelectedItem;

                //editorGrid.ExpandItemWithInitialExpandedAttribute();
                //editorGrid.BrowsableAttributes = new AttributeCollection(new UnitTypeAttribute(UnitType.Terran_Vulture));
            }
        }

        private void refreshListboxNames(ListBox list)
        {
            if (!listItemNameCache.ContainsKey(list)) listItemNameCache.Add(list, new Dictionary<object,String>());

            Dictionary<object, String> itemCache = listItemNameCache[list];
            for (int i = 0; i < list.Items.Count; ++i)
            {
                object item = list.Items[i];
                String newStr = item.ToString();

                if (!itemCache.ContainsKey(item)) itemCache.Add(item, newStr);
                if (itemCache[item].CompareTo(newStr) != 0)
                {
                    list.Items[i] = item;
                }
            }
        }

        private void timer_Tick(object sender, EventArgs e)
        {
            refreshListboxNames(unitList);
            refreshListboxNames(spriteList);
            refreshListboxNames(imageList);
            if (updateMemCheckbox.Checked)
            {
                editorGrid.Refresh();
                compareGrid.Refresh();
            }
        }

        private void compareBtn_Click(object sender, EventArgs e)
        {
            compareGrid.SelectedObject = editorGrid.SelectedObject;
        }
    }
}
