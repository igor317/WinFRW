#ifndef IPGRAPHICCOMPONENT_H
#define IPGRAPHICCOMPONENT_H

struct sRectangle
{
	int x;
	int y;
	int width;
	int height;

	sRectangle(int x, int y, int width, int height)
	{
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
	}
}

class cButton : IpGarPanel
{
	enum class ButtonNamePosition
	{
		Left,
		Center,
		Right,
	}
	public eNamePosition NamePosition{ get; private set; }
	protected int m_NameXOffset;
	protected int m_NameYOffset;
	protected string m_Name;

	public string Name
	{
		get { return m_Name; }
		set
		{
			m_Name = value;
			SetNamePosition(NamePosition);
		}
	}
		public override bool Enable
		{
			get { return m_Enable; }
			set
			{
				if (m_Enable == value)
					return;
				m_Enable = value;
				m_Selected = false;
				//BackColor = (m_Enable) ? Color.Gray : Color.DarkGray;
				DrawColor = (m_Enable) ? BackColor : InactiveColor;
				Draw();
			}
		}
			public override float FontSize
			{
				get { return m_Font.Size; }
				set
				{
					m_Font = new Font("Times New Roman", value);
					SetNamePosition(NamePosition);
				}
			}
			public delegate void MouseClickDelegate();
			public event MouseClickDelegate MouseClick;

			protected bool m_Selected;
			public Color TextColor;
			public Color InactiveColor;
			public Color SelectedColor;
			public Color ContourColor;
			public Color DrawColor;

			public cButton(string name, int x, int y, int width, int height, Control parent) : base(x, y, width, height, parent)
			{
				m_Name = name;
				FontSize = 18;
				SetNamePosition(eNamePosition.Center);
				//BackColor = Color.Gray;
				//InactiveColor = Color.DarkGray;
				BackColor = Color.DarkGray;
				InactiveColor = Color.Gray;
				SelectedColor = Color.Orange;
				TextColor = Color.Black;
				ContourColor = Color.Black;
				DrawColor = BackColor;

				m_Selected = false;
				m_Enable = true;

				m_Panel.MouseClick += MouseClickEvent;
				m_Panel.MouseLeave += MouseLeaveEvent;
				m_Panel.MouseEnter += MouseEnterEvent;
			}

			protected void MouseEnterEvent(object sender, EventArgs e)
			{
				if (Enable)
				{
					m_Selected = true;
					Draw();
				}
			}

			protected void MouseLeaveEvent(object sender, EventArgs e)
			{
				if (Enable)
				{
					m_Selected = false;
					Draw();
				}
			}

			protected virtual void MouseClickEvent(object sender, MouseEventArgs e)
			{
				ExecMouseClick();
			}

			protected override void DrawFunc()
			{
				//m_Brush.Color = (m_Panel.Enabled) ? ((m_Selected) ? SelectedColor : BackColor) : InactiveColor;
				m_Brush.Color = (m_Panel.Enabled) ? ((m_Selected) ? SelectedColor : DrawColor) : InactiveColor;
				m_Pen.Color = ContourColor;
				m_gBuff.FillRectangle(m_Brush, 0, 0, m_Panel.Width - 1, m_Panel.Height);
				m_gBuff.DrawRectangle(m_Pen, 0, 0, m_Panel.Width - 1, m_Panel.Height - 1);

				m_Brush.Color = TextColor;
				m_gBuff.DrawString(m_Name, m_Font, m_Brush, m_NameXOffset, m_NameYOffset);
			}

			public override void Show()
			{
				if (!m_Shown)
				{
					m_Shown = true;
					m_Panel.Show();
				}
			}

			public override void Hide()
			{
				if (m_Shown)
				{
					m_Shown = false;
					m_Panel.Hide();
				}
			}

			protected void ExecMouseClick()
			{
				if (Enable && MouseClick != null)
				{
					MouseClick();
					Draw();
				}

			}

			public void SetNamePosition(eNamePosition namePosition)
			{
				NamePosition = namePosition;
				m_NameYOffset = (m_Panel.Height - m_Font.Height) / 2;
				switch (NamePosition)
				{
				case eNamePosition.Left:
					m_NameXOffset = 0;
					break;
				case eNamePosition.Center:
					m_NameXOffset = m_Panel.Width / 2 - (int)m_gBuff.MeasureString(m_Name, m_Font).Width / 2;
					break;
				case eNamePosition.Right:
					m_NameXOffset = m_Panel.Width - (int)m_gBuff.MeasureString(m_Name, m_Font).Width;
					break;
				}
			}
}


#endif // !IPVERTICALSCROLL_H
