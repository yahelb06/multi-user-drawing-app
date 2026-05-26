using System;
using System.Collections.Generic;
using System.Windows;

namespace DrawingApp
{
    public partial class UploadWindow : Window
    {
        private List<string> _userPaints;
        public UploadWindow(List<string> paintsList)
        {
            InitializeComponent();

            _userPaints = paintsList;

            if (_userPaints != null && _userPaints.Count > 0)
            {
                PaintsListBox.ItemsSource = _userPaints;
            }
            else
            {
                MessageBox.Show("לא נמצאו ציורים שמורים.");
            }
        }

        private void ConfirmSelection_Click(object sender, RoutedEventArgs e)
        {
            if (PaintsListBox.SelectedItem is string selectedPaint)
            {
                ExecutePaintUpload(selectedPaint);
            }
            else
            {
                MessageBox.Show("אנא בחר ציור מהרשימה לפני האישור.");
            }
        }

        /// <summary>
        /// הפונקציה שביקשת - כאן קורה הביצוע של העלאת התמונה שנבחרה
        /// </summary>
        private void ExecutePaintUpload(string paintName)
        {
            this.DialogResult = true;
            this.Close();
        }

        private void ClosePopUp_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
    }
}