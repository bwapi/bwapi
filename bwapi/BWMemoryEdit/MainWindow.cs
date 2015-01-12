using Binarysharp.MemoryManagement;
using BWMemoryEdit.BW;
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
    /// <summary>
    /// Main window form
    /// </summary>
    public partial class MainWindow : Form
    {
        public static MainWindow instance = null;

        public static MemoryItem<Unit> Units = new MemoryItem<Unit>(0x0059CCA8, 336, 1700);
        public static MemoryItem<Sprite> Sprites = new MemoryItem<Sprite>(0x00629D98, 36, 2500);
        public static MemoryItem<Image> Images = new MemoryItem<Image>(0x0052F568, 64, 5000);
        public static MemoryItem<Image> ExtraImages = new MemoryItem<Image>(0x0057D768, 64, 80);

        private ItemController<Unit> UnitsController;
        private ItemController<Sprite> SpritesController;
        private ItemController<Image> ImagesController;
        private ItemController<Image> ExtraImagesController;

        /// <summary>
        /// A controller for associating a tab, list, and memory unit with each other.
        /// </summary>
        /// <typeparam name="T">The item that this controller manages (example: Unit, Sprite).</typeparam>
        class ItemController <T>
        {
            MemoryItem<T> memItem;
            TabPage tabPage;
            ListBox listBox;

            /// <summary>
            /// Constructor.
            /// </summary>
            /// <param name="memItem">The MemoryItem for validating and retrieving items from references.</param>
            /// <param name="tabPage">The tab page that the list is on.</param>
            /// <param name="list">The list containing all of the MemoryItem values.</param>
            public ItemController(MemoryItem<T> memItem, TabPage tabPage, ListBox list)
            {
                this.memItem = memItem;
                this.tabPage = tabPage;
                this.listBox = list;
            }

            /// <summary>
            /// Selects a reference in the UI after checking it for validity.
            /// </summary>
            /// <param name="reference">Reference to an item.</param>
            /// <returns>False if the provided reference is not valid in this context. True if it is valid and has been selected.</returns>
            public bool assignReference(object reference)
            {
                object item = memItem.GetItem(reference);
                if (item == null) return false;

                instance.offsetTabs.SelectedTab = tabPage;
                listBox.SelectedItem = item;
                instance.editorGrid.SelectedObject = item;

                return true;
            }
        }

        /// <summary>
        /// Main window constructor
        /// </summary>
        public MainWindow()
        {
            InitializeComponent();
            instance = this;

            Process[] processes = Process.GetProcessesByName("StarCraft");
            if (processes.Length > 0)
            {
                MemorySharp sharp = new MemorySharp(processes[0]);
                Units.InitList(unitList, sharp);
                Sprites.InitList(spriteList, sharp);
                Images.InitList(imageList, sharp);
                ExtraImages.InitList(extraImageList, sharp);
            }

            UnitsController = new ItemController<Unit>(Units, unitTab, unitList);
            SpritesController = new ItemController<Sprite>(Sprites, spriteTab, spriteList);
            ImagesController = new ItemController<Image>(Images, imageTab, imageList);
            ExtraImagesController = new ItemController<Image>(ExtraImages, extraImageTab, extraImageList);
        }

        /// <summary>
        /// Jumps to a Reference by selecting the item in the appropriate list, changing the tab, and selecting it into the main property grid.
        /// </summary>
        /// <param name="reference"></param>
        public void jumpToReference(object reference)
        {
            bool result =
                UnitsController.assignReference(reference) ||
                SpritesController.assignReference(reference) ||
                ImagesController.assignReference(reference) ||
                ExtraImagesController.assignReference(reference);
        }

        /// <summary>
        /// Callback for selecting items in any list.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void offsetList_SelectedIndexChanged(object sender, EventArgs e)
        {
            ListBox list = sender as ListBox;
            if (list != null && offsetTabs.SelectedTab.Contains(list))
            {
                editorGrid.SelectedObject = list.SelectedItem;
            }
        }

        private Dictionary<object, Dictionary<object, String>> listItemNameCache = new Dictionary<object, Dictionary<object, String>>();

        /// <summary>
        /// Refreshes the names in a listbox. Currently uses a workaround by reassigning the object.
        /// </summary>
        /// <param name="list"></param>
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

        /// <summary>
        /// A timer callback used to refresh data items in the list.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void refreshTimer_Tick(object sender, EventArgs e)
        {
            if (updateMemCheckbox.Checked)
            {
                refreshListboxNames(unitList);
                refreshListboxNames(spriteList);
                refreshListboxNames(imageList);
                refreshListboxNames(extraImageList);
                editorGrid.Refresh();
                compareGrid.Refresh();
            }
        }

        /// <summary>
        /// A callback for when the "Compare" button is clicked.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void compareBtn_Click(object sender, EventArgs e)
        {
            compareGrid.SelectedObject = editorGrid.SelectedObject;
        }
    }
}
