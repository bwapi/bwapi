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
        public static MemoryItem<Image> HPImages = new MemoryItem<Image>(0x0057EB78, 64, 12);

        private ItemController<Unit> UnitsController;
        private ItemController<Sprite> SpritesController;
        private ItemController<Image> ImagesController;
        private ItemController<Image> ExtraImagesController;
        private ItemController<Image> HPImagesController;

        public MemorySharp sharp;

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
                sharp = new MemorySharp(processes[0]);

                UnitsController = new ItemController<Unit>(Units, unitTab, unitList);
                SpritesController = new ItemController<Sprite>(Sprites, spriteTab, spriteList);
                ImagesController = new ItemController<Image>(Images, imageTab, imageList);
                ExtraImagesController = new ItemController<Image>(ExtraImages, extraImageTab, extraImageList);
                HPImagesController = new ItemController<Image>(HPImages, hpImageTab, hpImageList);
            }
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
                ExtraImagesController.assignReference(reference) ||
                HPImagesController.assignReference(reference);
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
        
        /// <summary>
        /// A timer callback used to refresh data items in the list.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void refreshTimer_Tick(object sender, EventArgs e)
        {
            if (updateMemCheckbox.Checked)
            {
                UnitsController.refresh();
                SpritesController.refresh();
                ImagesController.refresh();
                ExtraImagesController.refresh();
                HPImagesController.refresh();
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
